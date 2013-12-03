
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

#ifndef _FILESCANNER_H_
#define _FILESCANNER_H_

#include <QFile>

#include "scanner.h"

class cl_engine;

namespace Meduzzza
{
	class FileScanner : public Scanner
	{
		Q_OBJECT

	private:
		QString m_file;

	public:
		FileScanner(ClamavEngine *_engine, const QString &_file) : Scanner(_engine), m_file(_file) {}
		~FileScanner() {}

	protected:
		void runThread();

	Q_SIGNALS:
		void errorSignal(const QString &_file, const QString &_err);
	};
}

#endif
