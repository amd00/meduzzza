
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

#ifndef _PROCSCANNER_H_
#define _PROCSCANNER_H_

#include <QProcess>

#include "scanner.h"

class cl_engine;

namespace Meduzzza
{
	class ProcScanner : public Scanner
	{
		Q_OBJECT

	private:
		cl_engine *m_engine;
		Q_PID m_pid;

	public:
		ProcScanner(ClamavEngine *_engine, Q_PID &_pid) : Scanner(_engine), m_pid(_pid) {}
		~ProcScanner() {}

	protected:
		void runThread();

	Q_SIGNALS:
		void errorSignal(const QString &_name, Q_PID _pid, const QString &_err);
	};
}

#endif
