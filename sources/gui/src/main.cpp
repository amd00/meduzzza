
#include <QApplication>

#include "meduzzza.h"

int main(qint32 argc, char *argv[])
{
	QApplication app(argc, argv);
	
	Meduzzza m;
	m.show();
	
	return app.exec();
}