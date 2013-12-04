
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

#ifndef _DIRSCANNER_H_
#define _DIRSCANNER_H_

#include <QStringList>
#include <QDateTime>

#include "scanner.h"

class QThreadPool;

namespace Meduzzza
{
	class DirScanner : public Scanner
	{
	private:
		QString m_dir;
		QStringList m_excl_dirs;
		QThreadPool *m_pool;
		QDateTime m_time_start;
		quint64 m_files_count;

	public:
		DirScanner(ClamavEngine *_engine, const QString &_dir, const QStringList &_excl_dirs);
		~DirScanner();

	protected:
		void runThread();

	private:
		void scanDir(const QString &_dir);
	};
}
#endif
