
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
#include <QTextStream>
#include <QTemporaryFile>
#include <QThreadPool>

#include <clamav.h>

#include "memscanner.h"

namespace Meduzzza
{

	void MemScanner::run()
	{
		Q_EMIT memScanStartedSignal();
		scanMemory();
	}

	void MemScanner::scanMemory()
	{
		QDir proc_dir("/proc");
		QStringList procs = proc_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).filter(QRegExp("\\d+"));
		QRegExp mem_addr_regex("^([0-9a-fA-F]+)-([0-9a-fA-F]+)\\sr");
		foreach(QString proc, procs)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			QDir pr_dir(proc_dir.absoluteFilePath(proc));
			QString maps_file_str(pr_dir.absoluteFilePath("maps"));
			QFile maps_file(maps_file_str);
			qint32 proc_pid = proc.toInt();
			QString proc_name = QFileInfo(QFile::symLinkTarget(pr_dir.absoluteFilePath("exe"))).fileName();
			if(!maps_file.open(QIODevice::ReadOnly))
			{
				qCritical("ERROR: Open file error: %s", maps_file.errorString().toLocal8Bit().data());
				continue;
			}
			QTextStream maps_str(&maps_file);
			Q_EMIT procScanStartedSignal(proc_name, proc_pid);
			const char *virname = NULL;
			long unsigned int scanned = 0;
			qint32 result = 0;
			qDebug("INFO: Scanning process: %s(%i)", proc_name.toLocal8Bit().data(), proc_pid);
			for(QString line = maps_str.readLine(); !line.isNull(); line = maps_str.readLine())
			{
				qint32 pos = mem_addr_regex.indexIn(line);
				if(!pos)
				{
					quintptr start = mem_addr_regex.capturedTexts()[1].toUInt(NULL, 16);
					quintptr end = mem_addr_regex.capturedTexts()[2].toUInt(NULL, 16);
					cl_fmap_t *fmap = cl_fmap_open_memory((const void*)start, end - start);
					Q_ASSERT(fmap);
					result = cl_scanmap_callback(fmap, &virname, &scanned, m_engine, CL_SCAN_STDOPT, NULL);
					if(result == CL_VIRUS)
						break;
					cl_fmap_close(fmap);
				}
			}
			Q_EMIT procScanCompletedSignal(proc_name, proc_pid, result, virname);
		}
		Q_EMIT memScanCompletedSignal();
	}
}
