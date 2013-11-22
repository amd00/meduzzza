
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

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <QThread>
#include <QRunnable>

namespace Meduzzza
{
	class ScannerPrivate;
	
	class Sleeper : public QThread
	{
	public:
		static void usleep(unsigned long usecs) { QThread::usleep(usecs); }
		static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
		static void sleep(unsigned long secs) { QThread::sleep(secs); }
	};
	
	class Scanner : public QObject, public QRunnable
	{
	private:
		ScannerPrivate *m_p;

	public:
		Scanner();
		virtual ~Scanner();
		
		static void stop();
		static void pause();
		static void resume();
		
		static bool stopped();
		static void setStopped(bool _is_stopped);

	protected:
		void checkPause();
	};
}

#endif
