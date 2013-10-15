
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

#include <clamavengine.h>

#include "settings.h"
#include "dbengine.h"
#include "dbupdater.h"
#include "statist.h"

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
	Settings m_settings;
	DbEngine *m_db;
	DbUpdater *m_updater;
	bool m_full_scan_in_progress;
	Statist *m_statist;

public:
	Manager();
	~Manager();
	bool init();
	
	quint64 filesCount() const { return m_statist -> filesCount(); }
	quint64 procCount() const { return m_statist -> procCount(); }
	quint64 fileVirusesCount() const { return m_statist -> fileVirusesCount(); }
	quint64 procVirusesCount() const { return m_statist -> procVirusesCount(); }
	quint64 quarantined() const { return m_statist -> quarantined(); }
	quint32 virusDbAge() const { return m_engine -> dbAge(); }
	
public Q_SLOTS:
	void scanFile(const QString &_file, bool _non_block = true);
	void scanDir(const QString &_dir, const QStringList &_excl_dirs = QStringList(), bool _non_block = true);
	void scanMemory(bool _non_block = true);
	void fullScan();
	void updateDb(bool _non_block = true);
	void downloadDb(bool _non_block = true);
	void moveToQuarantine(const QString &_file, const QString &_virus);
	void stop();
	void pauseSlot() { m_engine -> pauseSlot(); }
	void resumeSlot() { m_engine -> resumeSlot(); }

private Q_SLOTS:
	void fileVirusDetectedSlot(const QString &_file, const QString &_virus);
	void memScanCompletedSlot();
	void dirScanCompletedSlot();
	void updateCompletedSlot();
	void updateErrorSlot(const QString &_file_name, const QString &_error_string);
	
Q_SIGNALS:
	void fileScanStartedSignal(const QString &_file);
	void procScanStartedSignal(const QString &_proc, qint32 _pid);
	void fileScanCompletedSignal(const QString &_file);
	void procScanCompletedSignal(const QString &_proc, qint32 _pid);
	void fileVirusDetectedSignal(const QString &_file, const QString &_virus);
	void procVirusDetectedSignal(const QString &_proc, qint32 _pid, const QString &_virus);
	void errorSignal(const QString &_file, const QString &_err);
	void fileMovedToQuarantineSignal(const QString &_source, const QString &_quarantined, const QString &_virus);
	void dbOutdatedSignal();
	void sigsLoadedSignal(quint32 _count);
	void fullScanCompletedSignal(const QDateTime &_time);
	void memScanStartedSignal();
	void dirScanStartedSignal();
	void memScanCompletedSignal();
	void dirScanCompletedSignal();
	void scanStoppedSignal();
	void downloadStartedSignal(const QString &_file);
	void downloadFinishedSignal(const QString &_file);
	void downloadProgressSignal(const QString &_file, qint64 _read, qint64 _total);
	void updateCompletedSignal();
};

#endif
