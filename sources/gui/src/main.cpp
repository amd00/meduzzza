
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

#include <QApplication>
#include <QFile>

#include "mainwindow.h"

int main(qint32 argc, char *argv[])
{
	QApplication app(argc, argv);
    QFile qss_file(":qss/stylesheet");
    qss_file.open(QIODevice::ReadOnly);
    QByteArray qss = qss_file.readAll();
    app.setStyleSheet(qss);
    
	QFont f = QApplication::font();
	f.setStyleStrategy(QFont::PreferAntialias);
	QApplication::setFont(f);
	
	Meduzzza::MainWindow m;
	m.show();
	
	return app.exec();
}