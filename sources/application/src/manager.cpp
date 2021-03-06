
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza.
 *
 *  meduzzza is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTemporaryFile>
#include <QEventLoop>

#include "manager.h"
#include "meduzzzaevent.h"

namespace Meduzzza
{
	
	Manager *Manager::ms_self = NULL;
	
	Manager *Manager::get()
	{
		if(ms_self == NULL)
			ms_self = new Manager();
		return ms_self;
	}

	Manager::Manager() : QObject(), m_app_dir(QDir::home().absoluteFilePath(".meduzzza")), m_db_dir(m_app_dir.absoluteFilePath("database")), 
						m_quarantine_dir(m_app_dir.absoluteFilePath("quarantine")),
						m_infected_db_dir(m_app_dir.absoluteFilePath("infdb")), m_engine(NULL), m_settings(new Settings(m_app_dir)), 
						m_db(new DbEngine(m_infected_db_dir)), m_updater(NULL), m_full_scan_in_progress(false), m_statist(new Statist())
	{
		if(!m_app_dir.mkpath(m_db_dir.dirName()))
			qCritical("ERROR: Unable create directory: %s", m_db_dir.absolutePath().toLocal8Bit().data());
		if(!m_app_dir.mkpath(m_quarantine_dir.dirName()))
			qCritical("ERROR: Unable create directory: %s", m_quarantine_dir.absolutePath().toLocal8Bit().data());
		if(!m_app_dir.mkpath(m_infected_db_dir.dirName()))
			qCritical("ERROR: Unable create directory: %s", m_infected_db_dir.absolutePath().toLocal8Bit().data());
		m_engine = new Meduzzza::ClamavEngine(m_settings -> threadCount(), m_db_dir.absolutePath());
		m_updater = new DbUpdater(this, m_settings -> dbUpdateMirror(), m_db_dir.absolutePath(), 
			m_settings -> proxyType(), m_settings -> proxyHost(), m_settings -> proxyPort(), m_settings -> proxyUser(), m_settings -> proxyPassword());
		
		connect(m_engine, SIGNAL(dirScanStartedSignal(const QString&, const QDateTime&)), 
				this, SIGNAL(dirScanStartedSignal(const QString&, const QDateTime&)));
		connect(m_engine, SIGNAL(dirScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
				this, SIGNAL(dirScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)));
		connect(m_engine, SIGNAL(dirScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
				this, SLOT(dirScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
		connect(m_engine, SIGNAL(memScanStartedSignal(const QDateTime&)), this, SIGNAL(memScanStartedSignal(const QDateTime&)));
		connect(m_engine, SIGNAL(memScanCompletedSignal(const QDateTime&, const QDateTime&)), 
				this, SIGNAL(memScanCompletedSignal(const QDateTime&, const QDateTime&)));
		connect(m_engine, SIGNAL(memScanCompletedSignal(const QDateTime&, const QDateTime&)), 
				this, SLOT(memScanCompletedSlot(const QDateTime&, const QDateTime&)));
		
		connect(m_engine, SIGNAL(fileScanStartedSignal(const QString&, const QDateTime&)), 
				this, SIGNAL(fileScanStartedSignal(const QString&, const QDateTime&)));
		connect(m_engine, SIGNAL(fileScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
				this, SIGNAL(fileScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)));
		connect(m_engine, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)), 
				this, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)));
		
		connect(m_engine, SIGNAL(procScanStartedSignal(const QString&, Q_PID, const QDateTime&)), 
				this, SIGNAL(procScanStartedSignal(const QString&, Q_PID, const QDateTime&)));
		connect(m_engine, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&)), 
				this, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&)));
		connect(m_engine, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)), 
				this, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)));
		connect(m_engine, SIGNAL(procScanErrorSignal(const QString&, Q_PID, const QString&)), 
				this, SIGNAL(procScanErrorSignal(const QString&, Q_PID, const QString&)));
		
		connect(m_engine, SIGNAL(stoppedSignal()), this, SIGNAL(stoppedSignal()));
		connect(m_engine, SIGNAL(pausedSignal()), this, SIGNAL(pausedSignal()));
		connect(m_engine, SIGNAL(resumedSignal()), this, SIGNAL(resumedSignal()));
		
		connect(m_engine, SIGNAL(filesFoundSignal(quint64)), this, SIGNAL(filesFoundSignal(quint64)));
		connect(m_engine, SIGNAL(procsFoundSignal(quint64)), this, SIGNAL(procsFoundSignal(quint64)));
		
		connect(this, SIGNAL(updateCompletedSignal(const QDateTime&, const QDateTime&)), this, SLOT(reset()));

		connect(this, SIGNAL(fileScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
				m_db, SLOT(fileScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
		connect(this, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)), 
				m_db, SLOT(fileVirusDetectedSlot(const QString&, const QDateTime&, const QDateTime&, const QString&)));
		connect(this, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&)), 
				m_db, SLOT(procScanCompletedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&)));
		connect(this, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)), 
				m_db, SLOT(procVirusDetectedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)));
		connect(this, SIGNAL(fileMovedToQuarantineSignal(const QString&, const QString&, const QString&)), 
				m_db, SLOT(fileMovedToQuarantineSlot(const QString&, const QString&, const QString&)));
		connect(this, SIGNAL(dirScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
				m_db, SLOT(dirScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
		connect(this, SIGNAL(memScanCompletedSignal(const QDateTime&, const QDateTime&)), 
				m_db, SLOT(memScanCompletedSlot(const QDateTime&, const QDateTime&)));
		connect(this, SIGNAL(fullScanCompletedSignal(const QDateTime&, const QDateTime&)), 
				m_db, SLOT(fullScanCompletedSlot(const QDateTime&, const QDateTime&)));
		connect(m_engine, SIGNAL(stoppedSignal()), m_db, SLOT(commit()));


		connect(this, SIGNAL(fileScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
				m_statist, SLOT(fileScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
		connect(this, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)), 
				m_statist, SLOT(fileVirusDetectedSlot(const QString&, const QDateTime&, const QDateTime&, const QString&)));
		connect(this, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&)), 
				m_statist, SLOT(procScanCompletedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&)));
		connect(this, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)), 
				m_statist, SLOT(procVirusDetectedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)));
		connect(m_engine, SIGNAL(procScanErrorSignal(const QString&, Q_PID, const QString&)), 
				m_statist, SLOT(procScanErrorSlot(const QString&, Q_PID, const QString&)));
		connect(this, SIGNAL(fileMovedToQuarantineSignal(const QString&, const QString&, const QString&)), 
				m_statist, SLOT(fileMovedToQuarantineSlot(const QString&, const QString&, const QString&)));
		
		connect(this, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)), 
				this, SLOT(fileVirusDetectedSlot(const QString&, const QDateTime&, const QDateTime&, const QString&)));
		
		connect(m_engine, SIGNAL(sigLoadStartedSignal()), this, SIGNAL(sigLoadStartedSignal()));
		connect(m_engine, SIGNAL(sigLoadCompletedSignal(qint32)), this, SIGNAL(sigLoadCompletedSignal(qint32)));
		connect(m_engine, SIGNAL(sigLoadErrorSignal()), this, SIGNAL(sigLoadErrorSignal()));
		
		connect(m_settings, SIGNAL(dbUpdateMirrorChangedSignal(const QString&)), m_updater, SLOT(dbUpdateMirrorChangedSlot(const QString&)));
		connect(m_settings, SIGNAL(proxyTypeChangedSignal(QNetworkProxy::ProxyType)), 
				m_updater, SLOT(proxyTypeChangedSlot(QNetworkProxy::ProxyType)));
		connect(m_settings, SIGNAL(proxyHostChangedSignal(const QString&)), m_updater, SLOT(proxyHostChangedSlot(const QString&)));
		connect(m_settings, SIGNAL(proxyPortChangedSignal(qint16)), m_updater, SLOT(proxyPortChangedSlot(qint16)));
		connect(m_settings, SIGNAL(proxyUserChangedSignal(const QString&)), m_updater, SLOT(proxyUserChangedSlot(const QString&)));
		connect(m_settings, SIGNAL(proxyPasswordChangedSignal(const QString&)), m_updater, SLOT(proxyPasswordChangedSlot(const QString&)));
	}

	Manager::~Manager()
	{
		delete m_engine;
		delete m_db;
		delete m_updater;
		delete m_statist;
		delete m_settings;
	}

	bool Manager::init()
	{
		qint32 db_age = m_engine -> dbAge();
		if(db_age > m_settings -> maxDbAge())
		{
			qWarning("WARNING: The virus database older than %i days.", m_settings -> maxDbAge());
			Q_EMIT void dbOutdatedSignal();
		}
		qint32 signo = -1;
		if(!m_engine -> init() || (signo = m_engine -> loadDb()) == -1 || !m_engine -> compile())
			qCritical("ERROR: Unable to init clamav engine");

		return true;
	}

	void Manager::scanFile(const QString &_file, bool _non_block)
	{
		QEventLoop loop;
		loop.connect(m_engine, SIGNAL(fileScanCompletedSignal(const QString&)), SLOT(quit()));
		loop.connect(m_engine, SIGNAL(scanStoppedSignal()), SLOT(quit()));

		m_engine -> scanFile(_file);
		
		if(!_non_block)
			loop.exec();
	}

	void Manager::scanDir(const QString &_dir, const QStringList &_excl_dirs, bool _non_block)
	{
		QEventLoop loop;
		loop.connect(m_engine, SIGNAL(dirScanCompletedSignal(const QString&)), SLOT(quit()));

		m_statist -> reset();
		m_db -> transaction();
		m_engine -> scanDir(_dir, QStringList() << _excl_dirs << m_quarantine_dir.absolutePath());

		if(!_non_block)
			loop.exec();
	}

	void Manager::scanMemory(bool _non_block)
	{
		QEventLoop loop;
		loop.connect(m_engine, SIGNAL(memScanCompletedSignal()), SLOT(quit()));
	
		m_db -> transaction();
		m_engine -> scanMemory();
	
		if(!_non_block)
			loop.exec();
	}

	void Manager::fullScan()
	{
		m_full_scan_time_start = QDateTime::currentDateTime();
		Q_EMIT fullScanStartedSignal(m_full_scan_time_start);
		m_full_scan_in_progress = true;
		scanMemory();
	}

	void Manager::updateDb(bool _non_block)
	{
		QEventLoop loop;
		loop.connect(m_updater, SIGNAL(updateCompletedSignal()), SLOT(quit()));

		m_updater -> dailyUpdate();
		
		if(!_non_block)
			loop.exec();
	}

	void Manager::downloadDb(bool _non_block)
	{
		QEventLoop loop;
		loop.connect(m_updater, SIGNAL(updateCompletedSignal()), SLOT(quit()));
		
		m_updater -> update();
		
		if(!_non_block)
			loop.exec();
	}

	void Manager::moveToQuarantine(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus)
	{
		char *tmp_file = tempnam(m_quarantine_dir.absolutePath().toLocal8Bit().data(), "amdq_");
		if(!tmp_file)
			return;
		QFile::rename(_file, tmp_file);
		Q_EMIT fileMovedToQuarantineSignal(_file, tmp_file, _virus);
		free(tmp_file);
	}
	
	void Manager::reset()
	{
		m_engine -> reset();
	}

	void Manager::stop()
	{
		m_full_scan_in_progress = false;
		m_engine -> stop();
	}
	
	bool Manager::event(QEvent *_event)
	{
		bool res = true;
		switch((MeduzzzaEvent::MeduzzzaEventType)_event -> type())
		{
		case MeduzzzaEvent::UpdateStarted:
			Q_EMIT updateStartedSignal(((UpdateStartedEvent*)_event) -> isFull(), 
									   ((UpdateStartedEvent*)_event) -> startTime());
			break;
		case MeduzzzaEvent::UpdateCompleted:
			Q_EMIT updateCompletedSignal(((UpdateCompletedEvent*)_event) -> startTime(), 
									   ((UpdateCompletedEvent*)_event) -> endTime());
			break;
		case MeduzzzaEvent::FileDownloadStarted:
			Q_EMIT fileDownloadStartedSignal(((FileDownloadStartedEvent*)_event) -> file(), 
									   ((FileDownloadStartedEvent*)_event) -> startTime());
			break;
		case MeduzzzaEvent::FileDownloadCompleted:
			Q_EMIT fileDownloadCompletedSignal(((FileDownloadCompletedEvent*)_event) -> file(), 
									   ((FileDownloadCompletedEvent*)_event) -> startTime(),
									   ((FileDownloadCompletedEvent*)_event) -> endTime());
			break;
		case MeduzzzaEvent::FileDownloadProgress:
			Q_EMIT fileDownloadProgressSignal(((FileDownloadProgressEvent*)_event) -> file(), 
									   ((FileDownloadProgressEvent*)_event) -> read(),
									   ((FileDownloadProgressEvent*)_event) -> total());
			break;
		case MeduzzzaEvent::FileDownloadError:
			Q_EMIT fileDownloadErrorSignal(((FileDownloadErrorEvent*)_event) -> file(), 
									   ((FileDownloadErrorEvent*)_event) -> error());
			break;
		default:
			res = QObject::event(_event);
			break;
		}
		return res;
	}
	

	void Manager::fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus)
	{
		if(m_settings -> autoQuarantine())
			moveToQuarantine(_file, _time_start, _time_end, _virus);
	}
	
	void Manager::memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end)
	{
		m_db -> commit();
		if(m_full_scan_in_progress)
			scanDir(QDir::rootPath(), QStringList() << "/proc" << "/sys" << "/dev");
	}

	void Manager::dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end)
	{
		m_db -> commit();
		if(m_full_scan_in_progress)
		{
			Q_EMIT fullScanCompletedSignal(m_full_scan_time_start, QDateTime::currentDateTime());
			m_full_scan_in_progress = false;
		}
	}

// 	void Manager::updateCompletedSlot()
// 	{
// 		qDebug("Virus databases updated!!!");
// 	}
// 
// 	void Manager::updateErrorSlot(const QString &_file_name, const QString &_error_string)
// 	{
// 		qCritical("ERROR: Database update error: %s - %s", _file_name.toLocal8Bit().data(), _error_string.toLocal8Bit().data());
// 	}

}
