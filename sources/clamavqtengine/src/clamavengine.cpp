
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
#include "scanevent.h"

namespace Meduzzza
{
	class ClamavEnginePrivate
	{
	private:
		QString m_db_path;
		cl_engine *m_engine;
		QThreadPool *m_pool;
		quint64 m_files_count;
		quint64 m_procs_count;
		
	public:
		ClamavEnginePrivate() : m_db_path(), m_engine(NULL), m_pool(QThreadPool::globalInstance()), 
			m_files_count(0), m_procs_count(0) {}
		~ClamavEnginePrivate()
		{
// 			delete m_pool;
			if(m_engine)
				cl_engine_free(m_engine);
		}
			
		QString dbPath() const { return m_db_path; }
		cl_engine *engine() const { return m_engine; }
		QThreadPool *pool() const { return m_pool; }
		quint64 filesCount() const { return m_files_count; }
		quint64 procsCount() const { return m_procs_count; }
		
		void setDbPath(const QString _db_path) { m_db_path = _db_path; }
		void setEngine(cl_engine *_engine) { m_engine = _engine; }
		void setFilesCount(quint64 _files_count) { m_files_count = _files_count; }
		void setProcsCount(quint64 _procs_count) { m_procs_count = _procs_count; }
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
	
	cl_engine *ClamavEngine::engine() const { return m_p -> engine(); }

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
		FileScanner *scanner = new FileScanner(this, _file);
		m_p -> pool() -> start(scanner);
		return true;
	}
	
	bool ClamavEngine::scanProcThread(Q_PID _pid)
	{
		ProcScanner *scanner = new ProcScanner(this, _pid);
		m_p -> pool() -> start(scanner);
		return true;
	}

	bool ClamavEngine::scanDirThread(const QString &_dir, const QStringList &_excl_dirs)
	{
		DirScanner *scanner = new DirScanner(this, _dir, _excl_dirs);
		connect(scanner, SIGNAL(filesFindedSignal(const QStringList&)), this, SLOT(filesFindedSlot(const QStringList&)));
		m_p -> pool() -> start(scanner);
		return true;
	}

	bool ClamavEngine::scanMemThread()
	{
		MemScanner *scanner = new MemScanner(this);
		connect(scanner, SIGNAL(procsFindedSignal(const PidList&)), this, SLOT(procsFindedSlot(const PidList&)));
		connect(scanner, SIGNAL(errorSignal(const QString&, Q_PID, const QString&)), this, SLOT(procScanErrorSlot(const QString&, Q_PID, const QString&)));
		m_p -> pool() -> start(scanner);
		return true;
	}

	bool ClamavEngine::scanFile(const QString &_file)
	{
		if(m_p -> pool() -> activeThreadCount())
			return false;
		return scanFileThread(_file);
	}

	bool ClamavEngine::scanDir(const QString &_dir, const QStringList &_excl_dirs)
	{
		if(m_p -> pool() -> activeThreadCount())
			return false;
		m_p -> setFilesCount(0);
		return scanDirThread(_dir, _excl_dirs);
	}

	bool ClamavEngine::scanMemory()
	{
		if(m_p -> pool() -> activeThreadCount())
			return false;
		m_p -> setProcsCount(0);
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
	
	bool ClamavEngine::event(QEvent *_event)
	{
		bool res = false;
		switch(_event -> type())
		{
			case ScanEvent::DirScanStarted:
				Q_EMIT dirScanStartedSignal(((DirScanStartedEvent*)_event) -> dir(), 
											((DirScanStartedEvent*)_event) -> startTime());
				res = true;
				break;
			case ScanEvent::DirScanCompleted:
				Q_EMIT dirScanCompletedSignal(((DirScanCompletedEvent*)_event) -> dir(), 
											  ((DirScanCompletedEvent*)_event) -> startTime(), 
											  QDateTime::currentDateTime());
				res = true;
				break;
			case ScanEvent::FileScanStarted:
				Q_EMIT fileScanStartedSignal(((FileScanStartedEvent*)_event) -> file(),
									   ((FileScanStartedEvent*)_event) -> startTime());
				res = true;
				break;
			case ScanEvent::FileScanCompleted:
				fileScanCompleted(((FileScanCompletedEvent*)_event) -> file(),
									  ((FileScanCompletedEvent*)_event) -> result(),
									  ((FileScanCompletedEvent*)_event) -> startTime(),
									  ((FileScanCompletedEvent*)_event) -> endTime(),
									  ((FileScanCompletedEvent*)_event) -> virname());
				res = true;
				break;
			case ScanEvent::MemScanStarted:
				Q_EMIT memScanStartedSignal(((MemScanStartedEvent*)_event) -> startTime());
				res = true;
				break;
			case ScanEvent::MemScanCompleted:
				Q_EMIT memScanCompletedSignal(((MemScanCompletedEvent*)_event) -> startTime(), 
											  QDateTime::currentDateTime());
				res = true;
				break;
			case ScanEvent::ProcScanStarted:
				Q_EMIT procScanStartedSignal(((ProcScanStartedEvent*)_event) -> name(),
											((ProcScanStartedEvent*)_event) -> pid(),
											((ProcScanStartedEvent*)_event) -> startTime());
				res = true;
				break;
			case ScanEvent::ProcScanCompleted:
				procScanCompleted(((ProcScanCompletedEvent*)_event) -> name(),
									((ProcScanCompletedEvent*)_event) -> pid(),
									((ProcScanCompletedEvent*)_event) -> result(),
									((ProcScanCompletedEvent*)_event) -> startTime(),
									((ProcScanCompletedEvent*)_event) -> endTime(),
									((ProcScanCompletedEvent*)_event) -> virname());
				res = true;
				break;
			default:
				res = QObject::event(_event);
		}
		return res;
	}

	qint32 ClamavEngine::loadSignature(const QString &_type, const QString &_name) const
	{
		Q_UNUSED(_type);
		Q_UNUSED(_name);
		return 0;
	}

	void ClamavEngine::fileScanCompleted(const QString &_file, qint32 _result, const QDateTime &_time_start, 
											 const QDateTime &_time_end, const QString &_virname)
	{
		switch(_result)
		{
			case CL_VIRUS:
				Q_EMIT fileVirusDetectedSignal(QFileInfo(_file).absoluteFilePath(), _time_start, _time_end, _virname);
				qDebug("INFO: End file scanning: %s: INFECTED - %s", QFileInfo(_file).absoluteFilePath().toLocal8Bit().data(), _virname.toLocal8Bit().data());
				break;
			case CL_CLEAN:
				Q_EMIT fileScanCompletedSignal(QFileInfo(_file).absoluteFilePath(), _time_start, _time_end);
				qDebug("INFO: End file scanning: %s: CLEAN", _file.toLocal8Bit().data());
				break;
			default:
				qDebug("INFO: Error - %s", cl_strerror(_result));
		}
	}
	
	void ClamavEngine::procScanCompleted(const QString &_name, Q_PID _pid, qint32 _result, const QDateTime &_time_start, 
											 const QDateTime &_time_end, const QString &_virname)
	{
		switch(_result)
		{
			case CL_VIRUS:
				Q_EMIT procVirusDetectedSignal(_name, _pid, _time_start, _time_end, _virname);
				qDebug("INFO: End process scanning: %s(%lli): INFECTED - %s", _name.toLocal8Bit().data(), _pid, _virname.toLocal8Bit().data());
				break;
			case CL_CLEAN:
				Q_EMIT procScanCompletedSignal(_name, _pid, _time_start, _time_end);
				qDebug("INFO: End process scanning: %s(%lli): CLEAN", _name.toLocal8Bit().data(), _pid);
				break;
			default:
				qDebug("INFO: Error - %s", cl_strerror(_result));
		}
	}
	
	void ClamavEngine::procScanErrorSlot(const QString &_name, Q_PID _pid, const QString &_error)
	{
		m_p -> setProcsCount(m_p -> procsCount() - 1);
		Q_EMIT procsFoundSignal(m_p -> procsCount());
	}

	void ClamavEngine::filesFindedSlot(const QStringList &_file_list)
	{
		m_p -> setFilesCount(m_p -> filesCount() + _file_list.size());
		Q_EMIT filesFoundSignal(m_p -> filesCount());
	}
	
	void ClamavEngine::procsFindedSlot(const PidList &_proc_list)
	{
		m_p -> setProcsCount(m_p -> procsCount() + _proc_list.size());
		Q_EMIT procsFoundSignal(m_p -> procsCount());
	}
	
}