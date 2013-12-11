
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

#include <QFileDialog>

#include <manager.h>

#include <ui_dirscanwidget.h>
#include <ui_meduzzzacommonwidget.h>

#include "mainwindow.h"
#include "dirscanwidget.h"
#include "meduzzzascanmodel.h"
#include "scanresdialog.h"

namespace Meduzzza
{

	DirScanWidget::DirScanWidget(MainWindow *_med) : 
			MeduzzzaCommonWidget(_med), m_ui(new Ui::DirScanWidget), m_mod(new MeduzzzaScanModel(false))
	{
		m_ui -> setupUi(m_base_ui -> m_widget);
		m_ui -> m_scan_view -> setModel(m_mod);
		m_ui -> m_scan_view -> setColumnHidden(MeduzzzaScanModel::Pid, true);
		
		connect(m_ui -> m_start_button, SIGNAL(clicked()), this, SLOT(startClickedSlot()));
		connect(m_ui -> m_stop_button, SIGNAL(clicked()), this, SLOT(stopClickedSlot()));
		connect(m_ui -> m_dir_button, SIGNAL(clicked()), this, SLOT(dirClickedSlot()));
	}

	DirScanWidget::~DirScanWidget() { delete m_ui; }

	void DirScanWidget::fileScanStarted(const QString &_file)
	{
	}

	void DirScanWidget::fileScanCompleted(const QString &_file)
	{
		m_ui -> m_slider -> setValue(m_ui -> m_slider -> value() + 1);
	}

	void DirScanWidget::fileVirusDetected(const QString &_file, const QString &_virus)
	{
		m_ui -> m_slider -> setValue(m_ui -> m_slider -> value() + 1);
	}

	void DirScanWidget::dirScanStarted(const QString &_dir)
	{
		m_ui -> m_slider -> setValue(0);
		m_mod -> clear();
		m_ui -> m_start_button -> setIcon(QIcon(":/images/pause"));
	}

	void DirScanWidget::dirScanCompleted(const QString &_dir)
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/play"));
		ScanResDialog res(false);
		res.exec();
	}
	
	void DirScanWidget::memScanStarted()
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}
	
	void DirScanWidget::memScanCompleted()
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}
		
	void DirScanWidget::fullScanStarted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}
	
	void DirScanWidget::fullScanCompleted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}

	void DirScanWidget::stopped()
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
		m_ui -> m_start_button -> setIcon(QIcon(":/images/play"));
		QApplication::restoreOverrideCursor();
	}

	void DirScanWidget::paused()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/play"));
	}

	void DirScanWidget::resumed()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/pause"));
	}
	
	void DirScanWidget::filesFound(quint64 _count)
	{
		m_ui -> m_slider -> setMaximum(_count);
	}
	
	void DirScanWidget::dirClickedSlot()
	{
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if(!dir.isEmpty())
			m_ui -> m_dir_edit -> setText(dir);
	}

	void DirScanWidget::startClickedSlot()
	{
		if(!isStarted())
			man() -> scanDir(m_ui -> m_dir_edit -> text());
		else if(!isPaused())
			man() -> pause();
		else
			man() -> resume();
	}
	
	void DirScanWidget::stopClickedSlot()
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_ui -> m_stop_button -> setDisabled(true);
		man() -> stop();
	}

	
}
