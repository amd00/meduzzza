
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
#include <QCoreApplication>
#include <QDateTime>
#include <QThreadPool>

#include "memscanner.h"
#include "procscanner.h"
#include "scanevent.h"

namespace Meduzzza
{
	
	MemScanner::MemScanner(ClamavEngine *_engine) : Scanner(_engine), m_pool(new QThreadPool), m_procs_count(0)
	{
		qint32 th_count = QThread::idealThreadCount();
		m_pool -> setMaxThreadCount(th_count <=0 ? 1 : th_count);
	}
		
	MemScanner::~MemScanner()
	{
		while(!m_pool -> waitForDone(10))
			QCoreApplication::processEvents();
		MemScanCompletedEvent *end_event(new MemScanCompletedEvent(m_start_time, QDateTime::currentDateTime()));
		QCoreApplication::postEvent((QObject*)engine(), end_event, end_event -> priority());
		delete m_pool;
	}

	void MemScanner::runThread()
	{
		QDateTime m_start_time = QDateTime::currentDateTime();
		MemScanStartedEvent *start_event(new MemScanStartedEvent(m_start_time));
		QCoreApplication::postEvent((QObject*)engine(), start_event, start_event -> priority());
		scanMemory();
	}

	void MemScanner::scanMemory()
	{
		QDir proc_dir("/proc");
		QStringList procs = proc_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).filter(QRegExp("\\d+"));
		m_procs_count = procs.size() - 1;
		ProcsFoundEvent *found_event(new ProcsFoundEvent(m_procs_count));
		QCoreApplication::postEvent((QObject*)engine(), found_event, found_event -> priority());
		foreach(QString proc, procs)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			Q_PID proc_pid = proc.toInt();
			if(proc_pid == QCoreApplication::applicationPid())
				continue;
			ProcScanner *scanner = new ProcScanner(engine(), proc_pid);
			m_pool -> start(scanner);
		}
	}
}
