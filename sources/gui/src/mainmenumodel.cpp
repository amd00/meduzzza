
#include "mainmenumodel.h"

namespace Meduzzza
{

	QVariant MainMenuModel::data(const QModelIndex &_index, int _role) const
	{
		QVariant res;
		switch(_role)
		{
			case Qt::DisplayRole:
				res = m_menu[_index.row()].text();
				break;
			case Qt::DecorationRole:
				res = m_menu[_index.row()].icon();
				break;
			case Qt::UserRole:
				res = QVariant::fromValue(m_menu[_index.row()].object());
				break;
		}
		return res;
	}

	QModelIndex MainMenuModel::index(int _row, int _column, const QModelIndex &_parent) const
	{
		if(_row < 0 || _row >= m_menu.size() || _column != 0)
			return QModelIndex();
		return createIndex(_row, _column, _row);
	}

	void MainMenuModel::addMenuItem(const QString &_text, const QString &_icon, QObject *_obj)
	{
		beginInsertRows(QModelIndex(), m_menu.size(), m_menu.size());
		m_menu.append(MainMenuItem(_text, _icon, _obj));
		endInsertRows();
	}

}