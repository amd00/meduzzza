
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

class DbEngine : public QObject
{
	Q_OBJECT

private:
	QSqlDatabase m_db;

public:
	DbEngine(const QDir &_dir);
	~DbEngine();
	void transaction() { m_db.transaction(); }
	void commit() { m_db.commit(); }

public Q_SLOTS:
	void fileMovedToQuarantineSlot(const QString &_source, const QString &_quarantined, const QString &_virus);
	void fullScanCompletedSlot(const QDateTime &_time);
	void fileVirusDetectedSlot(const QString &_file, const QString &_virus);
	void dirScanCompletedSlot(const QDateTime &_time, quint64 _files_count, quint64 _viruses_count);
};

#endif
