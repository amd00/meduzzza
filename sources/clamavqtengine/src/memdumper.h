
#ifndef _MEMDUMPER_H_
#define _MEMDUMPER_H_

#include <QProcess>

class QTemporaryFile;

namespace Meduzzza
{
	class MemDumper
	{
	private:
		Q_PID m_pid;
		
	public:
		MemDumper(Q_PID _pid) : m_pid(_pid) {}
		~MemDumper() {}
		
		bool dump(const QString &_file) const;
		QTemporaryFile *dump() const;
		
	private:
		bool dump(QFile *_file) const;
	};
}

#endif
