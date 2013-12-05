
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