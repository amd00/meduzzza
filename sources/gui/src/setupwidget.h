
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

#ifndef _SETUPWIDGET_H_
#define _SETUPWIDGET_H_

#include "meduzzzacommonwidget.h"

namespace Ui
{
	class SetupWidget;
}

namespace Meduzzza
{
	class Settings;
	
	class SetupWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::SetupWidget *m_ui;
		Settings *m_settings;
		
	public:
		SetupWidget(MainWindow *_med);
		~SetupWidget();
		
		QString text() const { return tr("Setup"); }
		
	private:
		void fileScanStarted(const QString &_file) {}
		void fileScanCompleted(const QString &_file) {}
		void fileVirusDetected(const QString &_file, const QString &_virname) {}
		
		void procScanStarted(const QString &_name, Q_PID _pid) {}
		void procScanCompleted(const QString &_name, Q_PID _pid) {}
		void procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname) {}
		void procScanError(const QString &_name, Q_PID _pid, const QString &_error) {}
		
		void memScanStarted() {}
		void memScanCompleted() {}
		
		void dirScanStarted(const QString &_dir) {}
		void dirScanCompleted(const QString &_dir) {}
		
		void stopped() {}
		void paused() {}
		void resumed() {}
		
		void filesFound(quint64 _count) {}
		void procsFound(quint64 _count) {}
		
		void fullScanStarted(const QDateTime &_time) {}
		void fullScanCompleted(const QDateTime &_time) {}
		
		void updateStarted(bool _is_full, const QDateTime &_start_time) {}
		void updateCompleted(const QDateTime &_start_time, const QDateTime &_end_time) {}
		void fileDownloadStarted(const QString &_file, const QDateTime &_start_time) {}
		void fileDownloadCompleted(const QString &_file, const QDateTime &_start_time, const QDateTime &_end_time) {}
		void fileDownloadProgress(const QString &_file, quint64 _read, quint64 _total) {}
		
		void sigLoadStarted() {}
		void sigLoadCompleted(qint32 _count) {}
		void sigLoadError() {}
		
	private Q_SLOTS:
		void autoquarStateChanged(qint32 _state);
		void proxytypeCurrentIndexChanged(qint32 _index);
		void proxysrvTextEdited(const QString &_text);
		void proxyportTextEdited(const QString &_text);
		void proxyuserTextEdited(const QString &_text);
		void proxypasswordTextEdited(const QString &_text);
		void dbsrvTextEdited(const QString &_text);
	};
}

#endif
