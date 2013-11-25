
#include <QTemporaryFile>
#include <QDir>
#include <QTextStream>

#include "memdumper.h"

namespace Meduzzza
{

	bool MemDumper::dump(const QString &_file) const
	{
		QFile file(_file);
		if(!file.open(QIODevice::WriteOnly))
			return false;
		bool res = dump(&file);
		file.close();
		if(!res)
			file.remove();
		return res;
	}
	
	QTemporaryFile *MemDumper::dump() const
	{
		QTemporaryFile *file = new QTemporaryFile();
		file -> open();
		bool res = dump(file);
		if(!res)
		{
			delete file;
			return NULL;
		}
		file -> seek(0);
		return file;
	}
	
	bool MemDumper::dump(QFile *_file) const
	{
#ifdef Q_WS_WIN
		return false;
#endif
		Q_CHECK_PTR(_file);
		Q_ASSERT(_file -> isOpen());
		QRegExp mem_addr_regex("^([0-9a-fA-F]+)-([0-9a-fA-F]+)\\sr");
		QDir system_proc_dir("/proc");
		QDir proc_dir(system_proc_dir.absoluteFilePath(QString::number(m_pid)));
		QString maps_file_str(proc_dir.absoluteFilePath("maps"));
		QString mem_file_str(proc_dir.absoluteFilePath("mem"));
		
		QFile maps_file(maps_file_str);
		QFile mem_file(mem_file_str);

		if(!maps_file.open(QIODevice::ReadOnly) || !mem_file.open(QIODevice::ReadOnly))
		{
			qCritical("ERROR: Open file error: %s", maps_file.errorString().toLocal8Bit().data());
			return false;
		}
		QTextStream maps_str(&maps_file);
		for(QString line = maps_str.readLine(); !line.isNull(); line = maps_str.readLine())
		{
			qint32 pos = mem_addr_regex.indexIn(line);
			if(!pos)
			{
				qint64 start = mem_addr_regex.capturedTexts()[1].toLongLong(NULL, 16);
				qint64 end = mem_addr_regex.capturedTexts()[2].toLongLong(NULL, 16);
				qint64 size = end - start;
				for(qint64 read = 0; read < size;)
				{
					start += read;
					mem_file.seek(start);
					QByteArray buf;
					buf = mem_file.read(size - read);
					if(buf.size() < size - read && mem_file.error() != QFile::NoError)
					{
						qCritical("ERROR: Open file error: %s", mem_file.errorString().toLocal8Bit().data());
						return false;
					}
					_file -> write(buf);
					read += buf.size();
				}
				
			}
		}
		return true;
	}

}
