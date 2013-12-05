
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza-gui.
 *
 *  meduzzza-gui is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza-gui is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza-gui.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MEDUZZZACOMMONWIDGET_H_
#define _MEDUZZZACOMMONWIDGET_H_

#include <QWidget>
#include <QProcess>

namespace Meduzzza
{
	class Manager;
	class MainWindow;
	
	class MeduzzzaCommonWidget : public QWidget
	{
		Q_OBJECT
		
	protected:
		Manager *m_man;
		MainWindow *m_mw;
		bool m_started;
		bool m_paused;
		
	public:
		MeduzzzaCommonWidget(MainWindow *_mw);
		virtual ~MeduzzzaCommonWidget() {}
		
	protected:
		virtual void fileScanStarted(const QString &_file) = 0;
		virtual void fileScanCompleted(const QString &_file) = 0;
		virtual void fileVirusDetected(const QString &_file, const QString &_virname) = 0;
		
		virtual void procScanStarted(const QString &_name, Q_PID _pid) = 0;
		virtual void procScanCompleted(const QString &_name, Q_PID _pid) = 0;
		virtual void procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname) = 0;
		virtual void procScanError(const QString &_name, Q_PID _pid, const QString &_error) = 0;
		
		virtual void dirScanStarted(const QString &_dir) = 0;
		virtual void dirScanCompleted(const QString &_dir) = 0;
		
		virtual void memScanStarted() = 0;
		virtual void memScanCompleted() = 0;
		
		virtual void fullScanStarted(const QDateTime &_time) = 0;
		virtual void fullScanCompleted(const QDateTime &_time) = 0;
		
		virtual void stopped() = 0;
		virtual void paused() = 0;
		virtual void resumed() = 0;
		
		virtual void filesFound(quint64 _count) = 0;
		virtual void procsFound(quint64 _count) = 0;
		
		virtual void updateStarted(bool _is_full, const QDateTime &_start_time) = 0;
		virtual void updateCompleted(const QDateTime &_start_time, const QDateTime &_end_time) = 0;
		virtual void fileDownloadStarted(const QString &_file, const QDateTime &_start_time) = 0;
		virtual void fileDownloadCompleted(const QString &_file, const QDateTime &_start_time, const QDateTime &_end_time) = 0;
		virtual void fileDownloadProgress(const QString &_file, quint64 _read, quint64 _total) = 0;
		
	private Q_SLOTS:
		void fileScanStartedSlot(const QString &_file, const QDateTime &_time_start) { fileScanStarted(_file); }
		void fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end) { fileScanCompleted(_file); }
		void fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virname) { fileVirusDetected(_file, _virname); }
		
		void procScanStartedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start) { procScanStarted(_name, _pid); }
		void procScanCompletedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end) { procScanCompleted(_name, _pid); }
		void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virname) { procVirusDetected(_name, _pid, _virname); }
		void procScanErrorSlot(const QString &_name, Q_PID _pid, const QString &_error) { procScanError(_name, _pid, _error); }
		
		void dirScanStartedSlot(const QString &_dir, const QDateTime &_time_start);
		void dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
		
		void memScanStartedSlot(const QDateTime &_time_start);
		void memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void fullScanStartedSlot(const QDateTime &_time_start);
		void fullScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void stoppedSlot();
		void pausedSlot();
		void resumedSlot();
		
		void filesFoundSlot(quint64 _count) { filesFound(_count); }
		void procsFoundSlot(quint64 _count) { procsFound(_count); }
		
		void updateStartedSlot(bool _is_full, const QDateTime &_start_time) { updateStarted(_is_full, _start_time); }
		void updateCompletedSlot(const QDateTime &_start_time, const QDateTime &_end_time) { updateCompleted(_start_time, _end_time); }
		void fileDownloadStartedSlot(const QString &_file, const QDateTime &_start_time) { fileDownloadStarted(_file, _start_time); }
		void fileDownloadCompletedSlot(const QString &_file, const QDateTime &_start_time, 
									   const QDateTime &_end_time) { fileDownloadCompleted(_file, _start_time, _end_time); }
		void fileDownloadProgressSlot(const QString &_file, quint64 _read, quint64 _total) { fileDownloadProgress(_file, _read, _total); }
	};
}
#endif
