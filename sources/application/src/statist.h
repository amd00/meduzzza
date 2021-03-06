
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza.
 *
 *  meduzzza is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _STATIST_H_
#define _STATIST_H_

#include <QObject>
#include <QProcess>

namespace Meduzzza
{
	class Statist : public QObject
	{
		Q_OBJECT
		
	private:
		quint64 m_files_count;
		quint64 m_proc_count;
		quint64 m_file_viruses_count;
		quint64 m_proc_viruses_count;
		quint64 m_proc_errors_count;
		quint64 m_quarantined;
		
	public:
		Statist() : QObject(), m_files_count(0), m_proc_count(0), 
				m_file_viruses_count(0), m_proc_viruses_count(0), m_proc_errors_count(0), m_quarantined(0) {}
		~Statist() {}
		
		quint64 filesCount() const { return m_files_count; }
		quint64 procsCount() const { return m_proc_count; }
		quint64 fileVirusesCount() const { return m_file_viruses_count; }
		quint64 procVirusesCount() const { return m_proc_viruses_count; }
		quint64 procErrorsCount() const { return m_proc_errors_count; }
		quint64 quarantined() const { return m_quarantined; }
		
		void reset();
		
	public Q_SLOTS:
		void fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end);
		void fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
		void procScanCompletedSlot(const QString &_proc, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end);
		void procVirusDetectedSlot(const QString &_proc, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virus);
		void procScanErrorSlot(const QString &_proc, Q_PID _pid, const QString &_error) { m_proc_errors_count++; }
		void fileMovedToQuarantineSlot(const QString &_source, const QString &_quarantined, const QString &_virus) 
		{ Q_UNUSED(_source) Q_UNUSED(_quarantined) Q_UNUSED(_virus) m_quarantined++; }
	};
}
#endif
