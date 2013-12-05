
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

#ifndef _MAINMENUMODEL_H_
#define _MAINMENUMODEL_H_

#include <QAbstractListModel>

#include "mainmenuitem.h"

namespace Meduzzza
{
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
		
		void addMenuItem(const QString &_text, const QString &_icon, QObject *_obj);
	};
}
#endif
