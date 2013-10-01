
#ifndef _MAINMENUMODEL_H_
#define _MAINMENUMODEL_H_

#include <QAbstractListModel>

#include "mainmenuitem.h"

Q_DECLARE_METATYPE(QSharedPointer<QObject>)

class MainMenuModel : public QAbstractListModel
{
	Q_OBJECT
	
private:
	MenuList m_menu;
	
public:
	MainMenuModel() : QAbstractListModel(), m_menu() {}
	~MainMenuModel() {}
	
	int columnCount(const QModelIndex &_parent = QModelIndex()) const { return 1; }
	int	rowCount (const QModelIndex &_parent = QModelIndex()) const { return m_menu.size(); }
	QVariant data(const QModelIndex &_index, int _role = Qt::DisplayRole) const;
	QModelIndex index(int _row, int _column, const QModelIndex &_parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &_index) const { return QModelIndex(); }
	
	void addMenuItem(const QString &_text, const QString &_icon, const QSharedPointer<QObject> &_obj);
};

#endif
