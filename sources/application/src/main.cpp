
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza.
 *
 *  meduzzza is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QCoreApplication>
#include <QTimer>
#include <getopt.h>

#include "manager.h"
int main(int argc, char *argv[])
{
 	
	QCoreApplication app(argc, argv);
	
	option long_opts[] =
	{
			{"update", no_argument, 0, 'u'},
			{"full-update", no_argument, 0, 'U'},
			{"memory", no_argument, 0, 'm'},
			{"file", required_argument, 0, 'f'},
			{"dir", required_argument, 0, 'd'},
			{"full-scan", no_argument, 0, 'F'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
	};
	char c = 0;
	int opt_index;
	optind = 1;
	opterr = 0;
	
	bool update = false;
	bool full_update = false;
	bool memory = false;
	QStringList files;
	QStringList dirs;
	bool full = false;
	
	while((c = getopt_long(argc, argv, "-u -U -m -f: -d: -F -h", long_opts, &opt_index)) >= 0)
	{
		switch(c)
		{
			case 'u':
			{
				update = true;
				break;
			}
			case 'U':
			{
				full_update = true;
				break;
			}
			case 'm':
			{
				memory = true;
				break;
			}
			case 'f':
			{
				files << optarg;
				break;
			}
			case 'd':
			{
				dirs << optarg;
				break;
			}
			case 'F':
			{
				full = true;
				break;
			}
			case 'h':
			{
				qDebug("Usage: meduzzza [-umhF] [-f <file name>] [-d <dir name>]");
				return 0;
			}
		}
	}
	
	if(full)
	{
		files.clear();
		dirs.clear();
		memory = false;
	}
	
	Meduzzza::Manager *m = Meduzzza::Manager::get();
	if(full_update)
	  m -> downloadDb(false);
	else if(update)
		m -> updateDb(false);
	m -> init();
	if(memory)
		m -> scanMemory(false);
	foreach(QString file, files)
		m -> scanFile(file, false);
	foreach(QString dir, dirs)
		m -> scanDir(dir, QStringList(), true);
	if(full)
		m -> fullScan();
	
	return app.exec();
}
