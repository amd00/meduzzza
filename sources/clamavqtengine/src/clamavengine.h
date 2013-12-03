
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of clamavqtengine.
 *
 *  clamavqtengine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  clamavqtengine is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with clamavqtengine.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CLAMAVENGINE_H_
#define _CLAMAVENGINE_H_

#include <QStringList>
#include <QProcess>

class QThreadPool;
typedef QList<Q_PID> PidList;

class cl_engine;

namespace Meduzzza
{
	class ClamavEnginePrivate;
	class ScanEvent;
	
	class ClamavEngine : public QObject
	{
		Q_OBJECT

	private:
		ClamavEnginePrivate *m_p;

	public:
		ClamavEngine(qint32 _thread_count = -1, const QString &_db_path = QString::null);
		virtual ~ClamavEngine();
		
		bool init();
		cl_engine *engine() const;
		
		qint32 dbAge() const;
		qint32 loadDb();
		bool compile();
		
		bool scanFile(const QString &_file);
		bool scanDir(const QString &_dir, const QStringList &_excl_dirs = QStringList());
		bool scanMemory();
		void stop();
		void pause();
		void resume();
		
		bool event(QEvent *_event);

	protected:
		virtual qint32 loadSignature(const QString &_type, const QString &_name) const;

	private:
		static int sigload_cb(const char *_type, const char *_name, quint32 _custom, void *_context);
		bool scanFileThread(const QString &_file);
		bool scanProcThread(Q_PID _pid);
		bool scanDirThread(const QString &_dir, const QStringList &_excl_dirs);
		bool scanMemThread();
		
		void fileScanCompleted(const QString &_file, qint32 _result, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virname);
		void procScanCompleted(const QString &_name, Q_PID _pid, qint32 _result, 
				const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virname);
		void dirScanCompleted(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
		void memScanCompleted(const QDateTime &_time_start, const QDateTime &_time_end);
		void procScanError(const QString &_name, Q_PID _pid, const QString &_error);

	private Q_SLOTS:
		void filesFindedSlot(const QStringList &_file_list);
		void procsFindedSlot(const PidList &_proc_list);
		
		void procScanErrorSlot(const QString &_name, Q_PID _pid, const QString &_error);

	Q_SIGNALS:
		void fileScanStartedSignal(const QString &_file, const QDateTime &_time_start);
		void fileScanCompletedSignal(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end);
		void fileVirusDetectedSignal(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virname);
		
		void procScanStartedSignal(const QString &_name, Q_PID _pid, const QDateTime &_time_start);
		void procScanCompletedSignal(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end);
		void procVirusDetectedSignal(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virname);
		
		void dirScanStartedSignal(const QString &_dir, const QDateTime &_time_start);
		void dirScanCompletedSignal(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
		
		void memScanStartedSignal(const QDateTime &_time_start);
		void memScanCompletedSignal(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void stoppedSignal();
		void pausedSignal();
		void resumedSignal();
		
		void filesFoundSignal(quint64 _count);
		void procsFoundSignal(quint64 _count);
	};
}
#endif
