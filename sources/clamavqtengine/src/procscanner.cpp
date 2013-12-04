
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

#include <QRegExp>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>

#include <clamav.h>

#include "procscanner.h"
#include "clamavengine.h"
#include "scanevent.h"

namespace Meduzzza
{

	void ProcScanner::runThread()
	{
		checkPause();
		if(Scanner::stopped())
			return;
		
		QRegExp mem_addr_regex("^([0-9a-fA-F]+)-([0-9a-fA-F]+)\\sr");
		QDir system_proc_dir("/proc");
		QDir proc_dir(system_proc_dir.absoluteFilePath(QString::number(m_pid)));
		QString maps_file_str(proc_dir.absoluteFilePath("maps"));
		QString mem_file_str(proc_dir.absoluteFilePath("mem"));
		QString proc_name = QFileInfo(QFile::symLinkTarget(proc_dir.absoluteFilePath("exe"))).fileName();
		QFile maps_file(maps_file_str);
		QFile mem_file(mem_file_str);
		
		QDateTime time_start = QDateTime::currentDateTime();
		ProcScanStartedEvent *start_event(new ProcScanStartedEvent(proc_name, m_pid, time_start));
		QCoreApplication::postEvent((QObject*)engine(), start_event, start_event -> priority());

		if(!maps_file.open(QIODevice::ReadOnly) || !mem_file.open(QIODevice::ReadOnly))
		{
			if(maps_file.error() != QFile::NoError)
			{
				qCritical("ERROR: Open file error: maps: %s", maps_file.errorString().toLocal8Bit().data());
// 				Q_EMIT errorSignal(proc_name, m_pid, maps_file.errorString());
				ProcScanErrorEvent *error_event(new ProcScanErrorEvent(proc_name, m_pid, maps_file.errorString()));
				QCoreApplication::postEvent((QObject*)engine(), error_event, error_event -> priority());
			}
			if(mem_file.error() != QFile::NoError)
			{
				qCritical("ERROR: Open file error: mem: %s", mem_file.errorString().toLocal8Bit().data());
// 				Q_EMIT errorSignal(proc_name, m_pid, mem_file.errorString());
				ProcScanErrorEvent *error_event(new ProcScanErrorEvent(proc_name, m_pid, maps_file.errorString()));
				QCoreApplication::postEvent((QObject*)engine(), error_event, error_event -> priority());
			}
			return;
		}
		QTextStream maps_str(&maps_file);
		long unsigned int scanned = 0;
		qint32 result = CL_CLEAN;
		const char *virname = NULL;
		
		qDebug("INFO: Scanning process: %s(%lli)", proc_name.toLocal8Bit().data(), m_pid);
		for(QString line = maps_str.readLine(); !line.isNull(); line = maps_str.readLine())
		{
			checkPause();
			if(Scanner::stopped())
				return;
			qint32 pos = mem_addr_regex.indexIn(line);
			if(!pos)
			{
				quint64 start = mem_addr_regex.capturedTexts()[1].toLongLong(NULL, 16);
				quint64 end = mem_addr_regex.capturedTexts()[2].toLongLong(NULL, 16);
				quint64 size = end - start;
				for(quint64 read = 0; read < size;)
				{
					checkPause();
					if(Scanner::stopped())
						return;
					start += read;
					mem_file.seek(start);
					QByteArray buf;
					buf = mem_file.read(size - read);
					if((quint64)buf.size() < size - read && mem_file.error() != QFile::NoError)
					{
						qCritical("ERROR: Read file error: mem: %s", mem_file.errorString().toLocal8Bit().data());
						read += size - read;
						continue;
					}
					
					cl_fmap_t *fmap = cl_fmap_open_memory(buf.data(), buf.size());
					result = cl_scanmap_callback(fmap, &virname, &scanned, engine() -> engine(), CL_SCAN_STDOPT, NULL);
					cl_fmap_close(fmap);
					if(result == CL_VIRUS)
						break;
					read += buf.size();
				}
				if(result == CL_VIRUS)
					break;
			}
		}
		maps_file.close();
		mem_file.close();
		ProcScanCompletedEvent *end_event(new ProcScanCompletedEvent(proc_name, m_pid, result, time_start, QDateTime::currentDateTime(), virname));
		QCoreApplication::postEvent((QObject*)engine(), end_event, end_event -> priority());
	}
}