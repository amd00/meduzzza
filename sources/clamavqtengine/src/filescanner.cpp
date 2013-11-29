
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

#include <QDateTime>
#include <QCoreApplication>

#include <clamav.h>

#include "filescanner.h"
#include "clamavengine.h"
#include "scanevent.h"

namespace Meduzzza
{

	void FileScanner::runThread()
	{
		checkPause();
		if(Scanner::stopped())
			return;
		
		qDebug("INFO: Scanning file: %s", m_file.toLocal8Bit().data());
		QDateTime time_start = QDateTime::currentDateTime();
		FileScanStartedEvent *start_event(new FileScanStartedEvent(m_file, time_start));
		QCoreApplication::postEvent((QObject*)engine(), start_event, start_event -> priority());
// 		Q_EMIT fileScanStartedSignal(m_file, time_start);

		const char *virname = NULL;
		long unsigned int scanned = 0;
		int result = cl_scanfile(m_file.toLocal8Bit().data(), &virname, &scanned, engine() -> engine(), CL_SCAN_STDOPT);
		FileScanCompletedEvent *end_event(new FileScanCompletedEvent(m_file, result, time_start, QDateTime::currentDateTime(),  virname));
		QCoreApplication::postEvent((QObject*)engine(), end_event, end_event -> priority());
// 		Q_EMIT fileScanCompletedSignal(m_file, result, time_start, QDateTime::currentDateTime(),  virname);
	}

}
