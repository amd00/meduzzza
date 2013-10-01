
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
	m_db.exec("create table if not exists quarantine_files(source text, quarantine text, virus text)");
	m_db.exec("create table if not exists full_scans(time text)");
	m_db.exec("create table if not exists scans(time text, files_count integer, viruses_count integer)");
	m_db.exec("create table if not exists detected_viruses(file text, virus text, time text)");
}

DbEngine::~DbEngine()
{
}

void DbEngine::fileMovedToQuarantineSlot(const QString &_source, const QString &_quarantined, const QString &_virus)
{
	QSqlQuery q(m_db);
	q.prepare("insert into quarantine_files (source, quarantine, virus) values (:source, :quarantine, :virus);");
	q.bindValue(":source", _source);
	q.bindValue(":quarantine", _quarantined);
	q.bindValue(":virus", _virus);
	q.exec();
}

void DbEngine::fullScanCompletedSlot(const QDateTime &_time)
{
	QSqlQuery q(m_db);
	q.prepare("insert into full_scans (time) values (:time);");
	q.bindValue(":time", _time.toString(Qt::ISODate));
	q.exec();
}

void DbEngine::fileVirusDetectedSlot(const QString &_file, const QString &_virus)
{
	QSqlQuery q(m_db);
	q.prepare("insert into detected_viruses (file, virus, time) values (:file, :virus, :time);");
	q.bindValue(":file", _file);
	q.bindValue(":virus", _virus);
	q.bindValue(":time", QDateTime::currentDateTime().toString(Qt::ISODate));
	q.exec();
}

void DbEngine::dirScanCompletedSlot(const QDateTime &_time, quint64 _files_count, quint64 _viruses_count)
{
	QSqlQuery q(m_db);
	q.prepare("insert into scans (time, files_count, viruses_count) values (:time, :files_count, :viruses_count);");
	q.bindValue(":time", _time);
	q.bindValue(":files_count", _files_count);
	q.bindValue(":viruses_count", _viruses_count);
	q.exec();
}
