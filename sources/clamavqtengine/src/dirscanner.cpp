
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

#include "dirscanner.h"

namespace Meduzzza
{

	void DirScanner::runThread()
	{
		scanDir(m_dir, true);
	}

	void DirScanner::scanDir(const QString &_dir, bool _top)
	{
		QDateTime time_start = QDateTime::currentDateTime();
		if(_top)
			Q_EMIT dirScanStartedSignal(m_dir, time_start);
		QDir dir(_dir);
		if(m_excl_dirs.contains(dir.absolutePath()))
			return;
		QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::NoSymLinks);
		foreach(QString d, dirs)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			scanDir(dir.absoluteFilePath(d), false);
		}
		QStringList files = dir.entryList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		QStringList file_list;
		foreach(QString f, files)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			file_list << dir.absoluteFilePath(f);
		}
		Q_EMIT filesFindedSignal(file_list);
		if(_top)
			Q_EMIT dirScanCompletedSignal(m_dir, time_start, QDateTime::currentDateTime());
	}
}
