
#include <QApplication>

#include "mainwindow.h"

int main(qint32 argc, char *argv[])
{
	QApplication app(argc, argv);
	
	Meduzzza::MainWindow m;
	m.show();
	
	return app.exec();
}