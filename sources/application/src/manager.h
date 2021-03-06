
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

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <QObject>
#include <QProcess>
#include <QDateTime>

#include <clamavengine.h>

#include "settings.h"
#include "dbengine.h"
#include "dbupdater.h"
#include "statist.h"

namespace Meduzzza
{
	class ClamavEngine;

	class Manager : public QObject
	{
		Q_OBJECT
		
	private:
		QDir m_app_dir;
		QDir m_db_dir;
		QDir m_quarantine_dir;
		QDir m_infected_db_dir;
		ClamavEngine *m_engine;
		Settings *m_settings;
		DbEngine *m_db;
		DbUpdater *m_updater;
		bool m_full_scan_in_progress;
		Statist *m_statist;
		QDateTime m_full_scan_time_start;
		
		static Manager *ms_self;
		
	public:
		static Manager *get();
		
		~Manager();
		bool init();
		
		quint64 filesCount() const { return m_statist -> filesCount(); }
		quint64 procsCount() const { return m_statist -> procsCount(); }
		quint64 fileVirusesCount() const { return m_statist -> fileVirusesCount(); }
		quint64 procVirusesCount() const { return m_statist -> procVirusesCount(); }
		quint64 quarantined() const { return m_statist -> quarantined(); }
		quint32 virusDbAge() const { return m_engine -> dbAge(); }
		
		void stop();
		void pause() { m_engine -> pause(); }
		void resume() { m_engine -> resume(); }
		
		Statist *statist() const { return m_statist; }
		Settings *settings() const { return m_settings; }
		
	protected:
		bool event(QEvent *_event);
		
	private:
		Manager();
		
	public Q_SLOTS:
		void scanFile(const QString &_file, bool _non_block = true);
		void scanDir(const QString &_dir, const QStringList &_excl_dirs = QStringList(), bool _non_block = true);
		void scanMemory(bool _non_block = true);
		void fullScan();
		void updateDb(bool _non_block = true);
		void downloadDb(bool _non_block = true);
		void moveToQuarantine(const QString &_file, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virus);
		void reset();

	private Q_SLOTS:
		void fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
// 		void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
		void dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
		void memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
// 		void updateCompletedSlot();
// 		void updateErrorSlot(const QString &_file_name, const QString &_error_string);
		
	Q_SIGNALS:
		void fileScanStartedSignal(const QString &_file, const QDateTime &_time_start);
		void fileScanCompletedSignal(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end);
		void fileVirusDetectedSignal(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
		
		void procScanStartedSignal(const QString &_name, Q_PID _pid, const QDateTime &_time_start);
		void procScanCompletedSignal(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end);
		void procVirusDetectedSignal(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
		void procScanErrorSignal(const QString &_name, Q_PID _pid, const QString &_error);
		
		void dirScanStartedSignal(const QString &_dir, const QDateTime &_time_start);
		void dirScanCompletedSignal(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
		
		void memScanStartedSignal(const QDateTime &_time_start);
		void memScanCompletedSignal(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void fullScanStartedSignal(const QDateTime &_time_start);
		void fullScanCompletedSignal(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void stoppedSignal();
		void pausedSignal();
		void resumedSignal();
		
		void filesFoundSignal(quint64 _count);
		void procsFoundSignal(quint64 _count);
		
		void updateStartedSignal(bool _is_full, const QDateTime &_start_time);
		void updateCompletedSignal(const QDateTime &_start_time, const QDateTime &_end_time);
		void fileDownloadStartedSignal(const QString &_file, const QDateTime &_start_time);
		void fileDownloadCompletedSignal(const QString &_file, const QDateTime &_start_time, const QDateTime &_end_time);
		void fileDownloadProgressSignal(const QString &_file, quint64 _read, quint64 _total);
		void fileDownloadErrorSignal(const QString &_file, const QString &_error);
		
		void sigLoadStartedSignal();
		void sigLoadCompletedSignal(qint32 _count);
		void sigLoadErrorSignal();
		
		void errorSignal(const QString &_file, const QString &_err);
		void fileMovedToQuarantineSignal(const QString &_source, const QString &_quarantined, const QString &_virus);
		void dbOutdatedSignal();
	};
}

#endif
