
#include "meduzzza.h"
#include "scanwidget.h"
#include "dirscanwidget.h"
#include "updatewidget.h"

Meduzzza::Meduzzza() : QWidget(), m_ui(), m_menu(), m_man()
{
	m_ui.setupUi(this);
	m_ui.m_menu_view -> setModel(&m_menu);
	
	QSharedPointer<ScanWidget> sw(new ScanWidget(&m_man, this, NULL));
	QSharedPointer<DirScanWidget> dsw(new DirScanWidget(&m_man, this, NULL));
	QSharedPointer<UpdateWidget> uw(new UpdateWidget(&m_man, this, NULL));
	m_menu.addMenuItem(sw -> text(), ":/images/images/item.png", sw.objectCast<QObject>());
	m_menu.addMenuItem(dsw -> text(), ":/images/images/item.png", dsw.objectCast<QObject>());
	m_menu.addMenuItem(uw -> text(), ":/images/images/item.png", uw.objectCast<QObject>());
	
	connect(m_ui.m_menu_view -> selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		this, SLOT(menuSelectedSlot(const QModelIndex&, const QModelIndex&)));
	
	m_man.init();
}

void Meduzzza::showWidget(QSharedPointer<QWidget> _w)
{ 
	if(!m_ui.m_main_widget -> children().isEmpty())
	{
		QWidget *win = qobject_cast<QWidget*>(m_ui.m_main_widget -> children()[0]);
		win -> setParent(NULL);
		win -> hide();
	}
	_w -> setParent(m_ui.m_main_widget);
	_w -> show();
}

void Meduzzza::menuSelectedSlot(const QModelIndex &_current, const QModelIndex &_previous)
{
	QSharedPointer<QObject> obj = m_menu.data(_current, Qt::UserRole).value<QSharedPointer<QObject> >();
	QSharedPointer<QWidget> w = obj.staticCast<QWidget>();
	showWidget(w);
}

// void Meduzzza::showStatisticSlot()
// {
// 	QSharedPointer<ScanResWidget> srw(new ScanResWidget(m_man.filesCount(), m_man.fileVirusesCount(), m_man.quarantined(), NULL));
// 	showWidget(srw);
// }