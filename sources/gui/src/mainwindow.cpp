
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

#include <ui_mainwindow.h>

#include "mainwindow.h"
#include "scanwidget.h"
#include "dirscanwidget.h"
#include "memscanwidget.h"
#include "updatewidget.h"
#include "setupwidget.h"
#include "mainmenumodel.h"

namespace Meduzzza
{

	MainWindow::MainWindow() : QWidget(), m_ui(new Ui::MainWindow), m_menu(new MainMenuModel), m_man(Meduzzza::Manager::get())
	{
		m_ui -> setupUi(this);
		m_ui -> m_menu_view -> setModel(m_menu);
		
		ScanWidget *sw = new ScanWidget(this);
		DirScanWidget *dsw = new DirScanWidget(this);
		MemScanWidget *msw = new MemScanWidget(this);
		UpdateWidget *uw = new UpdateWidget(this);
		SetupWidget *stw = new SetupWidget(this);
		m_menu -> addMenuItem(sw -> text(), ":/images/menuitem", sw);
		m_menu -> addMenuItem(dsw -> text(), ":/images/menuitem", dsw);
		m_menu -> addMenuItem(msw -> text(), ":/images/menuitem", msw);
		m_menu -> addMenuItem(uw -> text(), ":/images/menuitem", uw);
		m_menu -> addMenuItem(stw -> text(), ":/images/menuitem", stw);
		
		connect(m_ui -> m_menu_view -> selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(menuSelectedSlot(const QModelIndex&, const QModelIndex&)));
		
		m_man -> init();
	}
	
	MainWindow::~MainWindow()
	{
		delete m_ui;
		delete m_menu;
	}

	void MainWindow::showWidget(QWidget *_w)
	{ 
		if(m_ui -> m_main_widget_layout -> itemAt(0))
			m_ui -> m_main_widget_layout -> itemAt(0) -> widget() -> hide();
		m_ui -> m_main_widget_layout -> insertWidget(0, _w);
		_w -> show();
		m_ui -> m_splitter -> setStretchFactor(0, 0);
		m_ui -> m_splitter -> setStretchFactor(1, 1);
	}
	
	void MainWindow::closeEvent(QCloseEvent *_event)
	{
		m_man -> stop();
		QWidget::closeEvent(_event);
	}

	void MainWindow::menuSelectedSlot(const QModelIndex &_current, const QModelIndex &_previous)
	{
		QObject *obj = m_menu -> data(_current, Qt::UserRole).value<QObject*>();
		QWidget *w = (QWidget*)obj;
		showWidget(w);
	}

	// void Meduzzza::showStatisticSlot()
	// {
	// 	QSharedPointer<ScanResWidget> srw(new ScanResWidget(m_man.filesCount(), m_man.fileVirusesCount(), m_man.quarantined(), NULL));
	// 	showWidget(srw);
	// }
	
}