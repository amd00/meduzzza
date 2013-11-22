
#include <manager.h>

#include <ui_mainwindow.h>

#include "mainwindow.h"
#include "scanwidget.h"
#include "dirscanwidget.h"
#include "updatewidget.h"
#include "mainmenumodel.h"

namespace Meduzzza
{

	MainWindow::MainWindow() : QWidget(), m_ui(new Ui::MainWindow), m_menu(new MainMenuModel), m_man(new Meduzzza::Manager)
	{
		m_ui -> setupUi(this);
		m_ui -> m_menu_view -> setModel(m_menu);
		
		ScanWidget *sw = new ScanWidget(m_man, this, NULL);
		DirScanWidget *dsw = new DirScanWidget(m_man, this);
		UpdateWidget *uw = new UpdateWidget(m_man, this, NULL);
		m_menu -> addMenuItem(sw -> text(), ":/images/images/item.png", sw);
		m_menu -> addMenuItem(dsw -> text(), ":/images/images/item.png", dsw);
		m_menu -> addMenuItem(uw -> text(), ":/images/images/item.png", uw);
		
		connect(m_ui -> m_menu_view -> selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(menuSelectedSlot(const QModelIndex&, const QModelIndex&)));
		
		m_man -> init();
	}
	
	MainWindow::~MainWindow()
	{
		delete m_ui;
		delete m_menu;
		delete m_man;
	}

	void MainWindow::showWidget(QWidget *_w)
	{ 
		if(!m_ui -> m_main_widget -> children().isEmpty())
		{
			QWidget *win = qobject_cast<QWidget*>(m_ui -> m_main_widget -> children()[0]);
			win -> setParent(NULL);
			win -> hide();
		}
		_w -> setParent(m_ui -> m_main_widget);
		_w -> show();
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