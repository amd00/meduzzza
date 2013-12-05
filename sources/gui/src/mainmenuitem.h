
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

#ifndef _MAINMENUITEM_H_
#define _MAINMENUITEM_H_

#include <QIcon>

class MainMenuItem
{
private:
	QString m_text;
	QIcon m_icon;
	QObject *m_obj;
	
public:
	MainMenuItem(const QString &_text, const QString &_icon, QObject *_obj) :
			m_text(_text), m_icon(_icon), m_obj(_obj) {}
	~MainMenuItem() {}
	
	QString text() const { return m_text; }
	QIcon icon() const { return m_icon; }
	QObject *object() const { return m_obj; }
};

typedef QList<MainMenuItem> MenuList;

#endif
