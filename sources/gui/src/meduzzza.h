
#ifndef _MEDUZZZA_H_
#define _MEDUZZZA_H_

#include <QWidget>
#include <manager.h>

#include <ui_meduzzza.h>

#include "mainmenumodel.h"

class Meduzzza : public QWidget
{
	Q_OBJECT

private:
	Ui::Meduzzza m_ui;
	MainMenuModel m_menu;
	Manager m_man;
	
public:
	Meduzzza();
	~Meduzzza() {}
	
private Q_SLOTS:
	void showWidget(QSharedPointer<QWidget> _w);
	void menuSelectedSlot(const QModelIndex &_current, const QModelIndex &_previous);
	void showStatisticSlot();
};

#endif
