
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

#ifndef _DBENGINE_H_
#define _DBENGINE_H_

#include <QObject>
#include <QSqlDatabase>
#include <QDir>
#include <QProcess>
#include <QMap>

class DbEngine : public QObject
{
	Q_OBJECT

private:
	QSqlDatabase m_db;
	QMap<QString, QSqlQuery*> m_queries;

public:
	DbEngine(const QDir &_dir);
	~DbEngine();
	void transaction() { m_db.transaction(); }
	void commit() { m_db.commit(); }

public Q_SLOTS:
	void fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end);
	void procScanCompletedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end);
	void fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
	void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
	void fileMovedToQuarantineSlot(const QString &_file, const QString &_q_file, const QString &_virus);
	void dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
	void memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
	void fullScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
};

#endif
