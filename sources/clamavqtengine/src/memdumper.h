
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

#ifndef _MEMDUMPER_H_
#define _MEMDUMPER_H_

#include <QProcess>

class QTemporaryFile;

namespace Meduzzza
{
	class MemDumper
	{
	private:
		Q_PID m_pid;
		
	public:
		MemDumper(Q_PID _pid) : m_pid(_pid) {}
		~MemDumper() {}
		
		bool dump(const QString &_file) const;
		QTemporaryFile *dump() const;
		
	private:
		bool dump(QFile *_file) const;
	};
}

#endif
