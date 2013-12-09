
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

#include <manager.h>

#include <ui_memscanwidget.h>
#include <ui_meduzzzacommonwidget.h>

#include "mainwindow.h"
#include "memscanwidget.h"
#include "meduzzzascanmodel.h"
#include "scanresdialog.h"

namespace Meduzzza
{
	
	MemScanWidget::MemScanWidget(MainWindow *_med) : 
			MeduzzzaCommonWidget(_med), m_ui(new Ui::MemScanWidget), m_mod(new MeduzzzaScanModel(true))
	{
		m_ui -> setupUi(m_base_ui -> m_widget);
		m_ui -> m_scan_view -> setModel(m_mod);
		connect(m_ui -> m_start_button, SIGNAL(clicked()), this, SLOT(startClickedSlot()));
		connect(m_ui -> m_stop_button, SIGNAL(clicked()), this, SLOT(stopClickedSlot()));
	}

	MemScanWidget::~MemScanWidget() { delete m_ui; }

	void MemScanWidget::startClickedSlot()
	{
		if(!isStarted())
			man() -> scanMemory();
		else if(!isPaused())
			man() -> pause();
		else
			man() -> resume();
	}
	
	void MemScanWidget::stopClickedSlot()
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_ui -> m_stop_button -> setDisabled(true);
		man() -> stop();
	}
	
	void MemScanWidget::procScanStarted(const QString &_name, Q_PID _pid)
	{
	
	}

	void MemScanWidget::procScanCompleted(const QString &_name, Q_PID _pid)
	{
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1);
	}

	void MemScanWidget::procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virus)
	{
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1);
	}
	
	void MemScanWidget::procScanError(const QString &_name, Q_PID _pid, const QString &_error)
	{
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1);
	}
	
	void MemScanWidget::dirScanStarted(const QString &_dir)
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}

	void MemScanWidget::dirScanCompleted(const QString &_dir)
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}

	void MemScanWidget::memScanStarted()
	{
		m_ui -> m_progress -> setValue(0);
		m_mod -> clear();
		m_ui -> m_start_button -> setIcon(QIcon(":/images/pause"));
	}

	void MemScanWidget::memScanCompleted()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/play"));
		ScanResDialog res(true);
		res.exec();
	}
	
	void MemScanWidget::fullScanStarted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}
	
	void MemScanWidget::fullScanCompleted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}

	void MemScanWidget::stopped()
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
		m_ui -> m_start_button -> setIcon(QIcon(":/images/play"));
		QApplication::restoreOverrideCursor();
	}

	void MemScanWidget::paused()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/play"));
	}

	void MemScanWidget::resumed()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/pause"));
	}
	
	void MemScanWidget::procsFound(quint64 _count)
	{
		m_ui -> m_progress -> setMaximum(_count);
	}
	
}
