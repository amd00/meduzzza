
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of clamavqtengine.
 *
 *  clamavqtengine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  clamavqtengine is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with clamavqtengine.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QThreadPool>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>

#include <clamav.h>

#include "clamavengine.h"
#include "filescanner.h"
#include "procscanner.h"
#include "dirscanner.h"
#include "memscanner.h"

namespace Meduzzza
{
	class ClamavEnginePrivate
	{
	private:
		QString m_db_path;
		cl_engine *m_engine;
		QThreadPool *m_pool;
		
	public:
		ClamavEnginePrivate() : m_db_path(), m_engine(NULL), m_pool(new QThreadPool) {}
		~ClamavEnginePrivate()
		{
			delete m_pool;
			if(m_engine)
				cl_engine_free(m_engine);
		}
			
		QString dbPath() const { return m_db_path; }
		cl_engine *engine() const { return m_engine; }
		QThreadPool *pool() const { return m_pool; }
		
		void setDbPath(const QString _db_path) { m_db_path = _db_path; }
		void setEngine(cl_engine *_engine) { m_engine = _engine; }
	};

	
//====================================================================================================//
	
	
	int ClamavEngine::sigload_cb(const char *_type, const char *_name, quint32 _custom, void *_context)
	{
			return ((ClamavEngine*)_context) -> loadSignature(_type, _name);
	}

	ClamavEngine::ClamavEngine(qint32 _thread_count, const QString &_db_path) :QObject(), m_p(new ClamavEnginePrivate)
	{
		m_p -> setDbPath(_db_path.isNull() ? cl_retdbdir() : _db_path);
		qint32 thread_count = _thread_count <= 0 ? QThread::idealThreadCount() : _thread_count;
		thread_count = (thread_count == -1 ? 1 : thread_count);
		m_p -> pool() -> setMaxThreadCount(thread_count);
		qRegisterMetaType<PidList>("PidList");
		qRegisterMetaType<Q_PID>("Q_PID");
	}

	ClamavEngine::~ClamavEngine() 
	{ 
		stop();
		delete m_p; 
	}

	bool ClamavEngine::init()
	{
		qint32 init_res = cl_init(CL_INIT_DEFAULT);
		if(init_res)
		{
			qCritical("ERROR: Init error: %s", cl_strerror(init_res));
			return false;
		}
		m_p -> setEngine(cl_engine_new());
		return true;
	}

	qint32 ClamavEngine::dbAge() const
	{
		QDir db_dir(m_p -> dbPath());
		QStringList files = db_dir.entryList(QStringList() << "*.cvd" << "*.cld", QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		QDateTime res, tmp;
		foreach(QString file, files)
		{
			cl_cvd *cvd = cl_cvdhead(db_dir.absoluteFilePath(file).toLocal8Bit().data());
			if(!cvd)
				continue;
			tmp.setTime_t(cvd -> stime);
			cl_cvdfree(cvd);
			if(res.isNull() || tmp > res)
				res = tmp;
		}
		return res.date().daysTo(QDate::currentDate());
	}

	qint32 ClamavEngine::loadDb()
	{
		cl_engine_set_clcb_sigload(m_p -> engine(), ClamavEngine::sigload_cb, (void*)this);
		quint32 signo;
		int load_res = cl_load(m_p -> dbPath().toLocal8Bit().data(), m_p -> engine(), &signo, CL_DB_STDOPT);
		if(load_res)
		{
			qCritical("ERROR: Load error: %s", cl_strerror(load_res));
			return -1;
		}
		return signo;
	}

	bool ClamavEngine::compile()
	{
		int compile_res = cl_engine_compile(m_p -> engine());
		if(compile_res)
		{
			qCritical("ERROR: Compile error: %s", cl_strerror(compile_res));
			return false;
		}
		return true;
	}

	bool ClamavEngine::scanFileThread(const QString &_file)
	{
		FileScanner *scanner = new FileScanner(m_p -> engine(), _file);
		connect(scanner, SIGNAL(fileScanStartedSignal(const QString&)), this, SIGNAL(fileScanStartedSignal(const QString&)));
		connect(scanner, SIGNAL(fileScanCompletedSignal(const QString&, qint32, const QString&)), this, SLOT(fileScanCompletedSlot(const QString&, qint32, const QString&)));
		m_p -> pool() -> start(scanner);
		return true;
	}
	
	bool ClamavEngine::scanProcThread(Q_PID _pid)
	{
		ProcScanner *scanner = new ProcScanner(m_p -> engine(), _pid);
		connect(scanner, SIGNAL(procScanStartedSignal(const QString&, Q_PID)), this, SIGNAL(procScanStartedSignal(const QString&, Q_PID)));
		connect(scanner, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, qint32, const QString&)), 
				this, SLOT(procScanCompletedSlot(const QString&, Q_PID, qint32, const QString&)));
		m_p -> pool() -> start(scanner);
		return true;
	}

	bool ClamavEngine::scanDirThread(const QString &_dir, const QStringList &_excl_dirs)
	{
		DirScanner *scanner = new DirScanner(_dir, _excl_dirs);
		connect(scanner, SIGNAL(dirScanStartedSignal(const QString&)), this, SIGNAL(dirScanStartedSignal(const QString&)));
		connect(scanner, SIGNAL(dirScanCompletedSignal(const QString&)), this, SLOT(dirScanCompletedSlot(const QString&)));
		connect(scanner, SIGNAL(filesFindedSignal(const QStringList&)), this, SLOT(filesFindedSlot(const QStringList&)));
		m_p -> pool() -> start(scanner);
		return true;
	}

	bool ClamavEngine::scanMemThread()
	{
		MemScanner *scanner = new MemScanner();
		connect(scanner, SIGNAL(memScanStartedSignal()), this, SIGNAL(memScanStartedSignal()));
		connect(scanner, SIGNAL(memScanCompletedSignal()), this, SLOT(memScanCompletedSlot()));
// 		connect(scanner, SIGNAL(procsFindedSignal(const PidList&)), this, SLOT(procsFindedSlot(const PidList&)));
		connect(scanner, SIGNAL(procFindedSignal(Q_PID)), this, SLOT(procFindedSlot(Q_PID)));
		m_p -> pool() -> start(scanner);
		return true;
	}

	bool ClamavEngine::scanFile(const QString &_file)
	{
		return scanFileThread(_file);
	}

	bool ClamavEngine::scanDir(const QString &_dir, const QStringList &_excl_dirs)
	{
		return scanDirThread(_dir, _excl_dirs);
	}

	bool ClamavEngine::scanMemory()
	{
		return scanMemThread();
	}

	void ClamavEngine::stop()
	{
		Scanner::stop();
		while(!m_p -> pool() -> waitForDone(10))
			QCoreApplication::processEvents();
		Scanner::setStopped(false);
		Q_EMIT stoppedSignal();
	}
	
	void ClamavEngine::pause()
	{
		Scanner::pause();
		Q_EMIT pausedSignal();
	}
	
	void ClamavEngine::resume()
	{
		Scanner::resume();
		Q_EMIT resumedSignal();
	}

	qint32 ClamavEngine::loadSignature(const QString &_type, const QString &_name) const
	{
		Q_UNUSED(_type);
		Q_UNUSED(_name);
		return 0;
	}

	void ClamavEngine::fileScanCompletedSlot(const QString &_file, qint32 _result, const QString &_virname)
	{
		switch(_result)
		{
			case CL_VIRUS:
				Q_EMIT fileVirusDetectedSignal(QFileInfo(_file).absoluteFilePath(), QString(_virname));
				qDebug("INFO: End file scanning: %s: INFECTED - %s", QFileInfo(_file).absoluteFilePath().toLocal8Bit().data(), _virname.toLocal8Bit().data());
				break;
			case CL_CLEAN:
				Q_EMIT fileScanCompletedSignal(QFileInfo(_file).absoluteFilePath());
				qDebug("INFO: End file scanning: %s: CLEAN", _file.toLocal8Bit().data());
				break;
			default:
				qDebug("INFO: Error - %s", cl_strerror(_result));
		}
	}
	
	void ClamavEngine::procScanCompletedSlot(const QString &_name, Q_PID _pid, qint32 _result, const QString &_virname)
	{
		switch(_result)
		{
			case CL_VIRUS:
				Q_EMIT procVirusDetectedSignal(_name, _pid, _virname);
				qDebug("INFO: End process scanning: %s(%lli): INFECTED - %s", _name.toLocal8Bit().data(), _pid, _virname.toLocal8Bit().data());
				break;
			case CL_CLEAN:
				Q_EMIT procScanCompletedSignal(_name, _pid);
				qDebug("INFO: End process scanning: %s(%lli): CLEAN", _name.toLocal8Bit().data(), _pid);
				break;
			default:
				qDebug("INFO: Error - %s", cl_strerror(_result));
		}
	}
	
	void ClamavEngine::dirScanCompletedSlot(const QString &_dir)
	{
		while(!m_p -> pool() -> waitForDone(10))
			QCoreApplication::processEvents();
		Q_EMIT dirScanCompletedSignal(_dir);
	}
	
	void ClamavEngine::memScanCompletedSlot()
	{
		while(!m_p -> pool() -> waitForDone(10))
			QCoreApplication::processEvents();
		Q_EMIT memScanCompletedSignal();
	}

	void ClamavEngine::filesFindedSlot(const QStringList &_file_list)
	{
		foreach(QString file, _file_list)
			scanFileThread(file);
	}

	void ClamavEngine::procsFindedSlot(const PidList &_proc_list)
	{
		foreach(Q_PID pid, _proc_list)
			scanProcThread(pid);
	}
	
	void ClamavEngine::procFindedSlot(Q_PID _pid)
	{
		scanProcThread(_pid);
	}

// 	void ClamavEngine::memScanCompletedSlot()
// 	{
// 		m_mem_scan = false;
// 		if(!m_count)
// 			Q_EMIT memScanCompletedSignal();
// 	}
// 
// 	void ClamavEngine::dirScanCompletedSlot()
// 	{
// 		m_dir_scan = false;
// 		if(!m_count)
// 			Q_EMIT dirScanCompletedSignal();
// 	}

}