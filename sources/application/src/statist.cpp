
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

#include "statist.h"

void Statist::reset()
{
	m_files_count = 0;
	m_proc_count = 0;
	m_file_viruses_count = 0;
	m_proc_viruses_count = 0;
	m_quarantined = 0;
}

void Statist::fileScanCompletedSlot(const QString &_file)
{
	Q_UNUSED(_file)
	m_files_count++;
}

void Statist::fileVirusDetectedSlot(const QString &_file, const QString &_virus)
{
	Q_UNUSED(_file)
	Q_UNUSED(_virus)
	m_file_viruses_count++;
}

void Statist::procScanCompletedSlot(const QString &_proc, qint32 _pid)
{
	Q_UNUSED(_proc)
	Q_UNUSED(_pid)
	m_proc_count++;
}

void Statist::procVirusDetectedSlot(const QString &_proc, qint32 _pid, const QString &_virus)
{
	Q_UNUSED(_proc)
	Q_UNUSED(_pid)
	Q_UNUSED(_virus)
	m_proc_viruses_count++;
}
