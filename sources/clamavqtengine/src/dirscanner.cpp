
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

#include <QDir>
#include <QDateTime>
#include <QCoreApplication>
#include <QThreadPool>

#include "dirscanner.h"
#include "filescanner.h"
#include "scanevent.h"

namespace Meduzzza
{
	DirScanner::DirScanner(ClamavEngine *_engine, const QString &_dir, const QStringList &_excl_dirs)  : Scanner(_engine), 
				m_dir(_dir), m_excl_dirs(_excl_dirs), m_pool(new QThreadPool), m_files_count(0)
	{
		qint32 th_count = QThread::idealThreadCount();
		m_pool -> setMaxThreadCount(th_count <=0 ? 1 : th_count);
	}
		
	DirScanner::~DirScanner() 
	{
		m_pool -> waitForDone();
		DirScanCompletedEvent *event(new DirScanCompletedEvent(m_dir, m_time_start, QDateTime::currentDateTime()));
		QCoreApplication::postEvent((QObject*)engine(), event, event -> priority());
		delete m_pool; 
	}

	void DirScanner::runThread()
	{
		m_time_start = QDateTime::currentDateTime();
		DirScanStartedEvent *event(new DirScanStartedEvent(m_dir, m_time_start));
		QCoreApplication::postEvent((QObject*)engine(), event, event -> priority());
		scanDir(m_dir);
	}

	void DirScanner::scanDir(const QString &_dir)
	{
		QDir dir(_dir);
		if(m_excl_dirs.contains(dir.absolutePath()))
			return;
		QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::NoSymLinks);
		foreach(QString d, dirs)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			scanDir(dir.absoluteFilePath(d));
		}
		QStringList files = dir.entryList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		m_files_count += files.size();
		FilesFoundEvent *found_event(new FilesFoundEvent(m_files_count));
		QCoreApplication::postEvent((QObject*)engine(), found_event, found_event -> priority());
// 		Q_EMIT filesFoundSignal(m_files_count);
		foreach(QString f, files)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			QString file = dir.absoluteFilePath(f);
			FileScanner *scanner = new FileScanner(engine(), file);
			m_pool -> start(scanner);
		}
	}
}
