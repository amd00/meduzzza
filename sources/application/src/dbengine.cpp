
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

#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>

#include "dbengine.h"

DbEngine::DbEngine(const QDir &_dir) : QObject(), m_db(QSqlDatabase::addDatabase("QSQLITE", "infected-database"))
{
	m_db.setDatabaseName(_dir.absoluteFilePath("infected_db.sqlite"));
	if(!m_db.open())
		qCritical("ERROR: %s", m_db.lastError().text().toLocal8Bit().data());
	m_db.exec("create table if not exists scanned_files(file text, time_start text, time_end text, virus text)");
	m_db.exec("create table if not exists scanned_procs(name text, pid integer, time_start text, time_end text, virus text)");
	m_db.exec("create table if not exists quarantined_files(file text, q_file text, virus text)");
	m_db.exec("create table if not exists dir_scans(dir text, time_start text, time_end text)");
	m_db.exec("create table if not exists mem_scans(time_start text, time_end text)");
	m_db.exec("create table if not exists full_scans(time_start text, time_end text)");
	
	QSqlQuery *scanned_files_ins = new QSqlQuery(m_db);
	if(!scanned_files_ins -> prepare("insert into scanned_files(file, time_start, time_end, virus) values(:file, :time_start, :time_end, :virus)"))
	{
		qCritical("ERROR: Unable to prepare query: %s", scanned_files_ins -> lastQuery().toLocal8Bit().data());
		delete scanned_files_ins;
	}
	else
		m_queries["scanned_files_ins"] = scanned_files_ins;
	QSqlQuery *scanned_procs_ins = new QSqlQuery(m_db);
	if(!scanned_procs_ins -> prepare("insert into scanned_procs(name, pid, time_start, time_end, virus) values(:name, :pid, :time_start, :time_end, :virus)"))
	{
		qCritical("ERROR: Unable to prepare query: %s", scanned_procs_ins -> lastQuery().toLocal8Bit().data());
		delete scanned_procs_ins;
	}
	else
		m_queries["scanned_procs_ins"] = scanned_procs_ins;
	QSqlQuery *quarantined_files_ins = new QSqlQuery(m_db);
	if(!quarantined_files_ins -> prepare("insert into quarantined_files(file, q_file, virus) values(:file, :q_file, :virus)"))
	{
		qCritical("ERROR: Unable to prepare query: %s", quarantined_files_ins -> lastQuery().toLocal8Bit().data());
		delete quarantined_files_ins;
	}
	else
		m_queries["quarantined_files_ins"] = quarantined_files_ins;
	QSqlQuery *dir_scans_ins = new QSqlQuery(m_db);
	if(!dir_scans_ins -> prepare("insert into dir_scans(dir, time_start, time_end) values(:dir, :time_start, :time_end)"))
	{
		qCritical("ERROR: Unable to prepare query: %s", dir_scans_ins -> lastQuery().toLocal8Bit().data());
		delete dir_scans_ins;
	}
	else
		m_queries["dir_scans_ins"] = dir_scans_ins;
	QSqlQuery *mem_scans_ins = new QSqlQuery(m_db);
	if(!mem_scans_ins -> prepare("insert into mem_scans(time_start, time_end) values(:time_start, :time_end)"))
	{
		qCritical("ERROR: Unable to prepare query: %s", mem_scans_ins -> lastQuery().toLocal8Bit().data());
		delete mem_scans_ins;
	}
	else
		m_queries["mem_scans_ins"] = mem_scans_ins;
	QSqlQuery *full_scans_ins = new QSqlQuery(m_db);
	if(!full_scans_ins -> prepare("insert into full_scans(time_start, time_end) values(:time_start, :time_end)"))
	{
		qCritical("ERROR: Unable to prepare query: %s", full_scans_ins -> lastQuery().toLocal8Bit().data());
		delete full_scans_ins;
	}
	else
		m_queries["full_scans_ins"] = full_scans_ins;
	
}

DbEngine::~DbEngine()
{
}

void DbEngine::fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end)
{
	QSqlQuery *q = m_queries["scanned_files_ins"];
	q -> bindValue(":file", _file);
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> bindValue(":virus", QVariant());
	q -> exec();
}

void DbEngine::procScanCompletedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end) 
{
	QSqlQuery *q = m_queries["scanned_procs_ins"];
	q -> bindValue(":name", _name);
	q -> bindValue(":pid", _pid);
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> bindValue(":virus", QVariant());
	q -> exec();
}

void DbEngine::fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus)
{
	QSqlQuery *q = m_queries["scanned_files_ins"];
	q -> bindValue(":file", _file);
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> bindValue(":virus", _virus);
	q -> exec();
}

void DbEngine::procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus) 
{
	QSqlQuery *q = m_queries["scanned_procs_ins"];
	q -> bindValue(":name", _name);
	q -> bindValue(":pid", _pid);
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> bindValue(":virus", _virus);
	q -> exec();
}

void DbEngine::fileMovedToQuarantineSlot(const QString &_file, const QString &_q_file, const QString &_virus) 
{
	QSqlQuery *q = m_queries["quarantined_files_ins"];
	q -> bindValue(":file", _file);
	q -> bindValue(":q_file", _q_file);
	q -> bindValue(":virus", _virus);
	q -> exec();
}

void DbEngine::dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end) 
{
	QSqlQuery *q = m_queries["dir_scans_ins"];
	q -> bindValue(":dir", _dir);
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> exec();
}

void DbEngine::memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end)
{
	QSqlQuery *q = m_queries["mem_scans_ins"];
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> exec();
}

void DbEngine::fullScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end)
{
	QSqlQuery *q = m_queries["full_scans_ins"];
	q -> bindValue(":time_start", _time_start.toString(Qt::ISODate));
	q -> bindValue(":time_end", _time_end.toString(Qt::ISODate));
	q -> exec();
}

