
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
#include <QCoreApplication>

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
		foreach(QString proc, procs)
		{
			checkPause();
			if(Scanner::stopped())
				return;
			QDir pr_dir(proc_dir.absoluteFilePath(proc));
			QString maps_file_str(pr_dir.absoluteFilePath("maps"));
			QFile maps_file(maps_file_str);
			Q_PID proc_pid = proc.toInt();
			if(proc_pid == QCoreApplication::applicationPid())
				continue;
			QString proc_name = QFileInfo(QFile::symLinkTarget(pr_dir.absoluteFilePath("exe"))).fileName();
			const char *virname = NULL;
			qint32 result = 0;
			qDebug("INFO: Scanning process: %s(%lli)", proc_name.toLocal8Bit().data(), proc_pid);
			Q_EMIT procScanStartedSignal(proc_name, proc_pid);
			result = scanProcess(proc_pid, &virname);
			Q_EMIT procScanCompletedSignal(proc_name, proc_pid, result, virname);
		}
		Q_EMIT memScanCompletedSignal();
	}
	
	qint32 MemScanner::scanProcess(Q_PID _pid, const char **_virname)
	{
		QRegExp mem_addr_regex("^([0-9a-fA-F]+)-([0-9a-fA-F]+)\\sr");
		QDir system_proc_dir("/proc");
		QDir proc_dir(system_proc_dir.absoluteFilePath(QString::number(_pid)));
		QString maps_file_str(proc_dir.absoluteFilePath("maps"));
		QString mem_file_str(proc_dir.absoluteFilePath("mem"));
		
		QFile maps_file(maps_file_str);
		QFile mem_file(mem_file_str);

		if(!maps_file.open(QIODevice::ReadOnly) || !mem_file.open(QIODevice::ReadOnly))
		{
			qCritical("ERROR: Open file error: %s", maps_file.errorString().toLocal8Bit().data());
			return -1;
		}
		QTextStream maps_str(&maps_file);
		long unsigned int scanned = 0;
		for(QString line = maps_str.readLine(); !line.isNull(); line = maps_str.readLine())
		{
			qint32 pos = mem_addr_regex.indexIn(line);
			if(!pos)
			{
				qint64 start = mem_addr_regex.capturedTexts()[1].toLongLong(NULL, 16);
				qint64 end = mem_addr_regex.capturedTexts()[2].toLongLong(NULL, 16);
				qint64 size = end - start;
				for(qint64 read = 0; read < size;)
				{
					start += read;
					mem_file.seek(start);
					QByteArray buf;
					buf = mem_file.read(size - read);
					if(buf.size() < size - read && mem_file.error() != QFile::NoError)
					{
						qCritical("ERROR: Open file error: %s", mem_file.errorString().toLocal8Bit().data());
						return -1;
					}
					
					cl_fmap_t *fmap = cl_fmap_open_memory(buf.data(), buf.size());
					qint32 result = cl_scanmap_callback(fmap, _virname, &scanned, m_engine, CL_SCAN_STDOPT, NULL);
					cl_fmap_close(fmap);
					if(result == CL_VIRUS)
						return result;
					read += buf.size();
				}
				
			}
		}
		return CL_CLEAN;
	}
}
