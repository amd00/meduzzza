
#ifndef _SCANEVENT_H_
#define _SCANEVENT_H_

#include <QDateTime>
#include <QProcess>
#include <QEvent>

namespace Meduzzza
{

	class ScanEvent : public QEvent
	{
	public:
		enum ScanEventType
		{
			FileScanStarted = QEvent::User + 12345,
			FileScanCompleted,
			ProcScanStarted,
			ProcScanCompleted,
			DirScanStarted,
			DirScanCompleted,
			MemScanStarted,
			MemScanCompleted
		};
		
	private:
		qint32 m_priority;
		
	public:
		ScanEvent(qint32 _type, qint32 _priority) : QEvent((QEvent::Type)_type), m_priority(_priority) {}
		virtual ~ScanEvent() {}
		
		qint32 priority() const { return m_priority; }
	};

	class FileScanStartedEvent : public ScanEvent
	{
	private:
		QString m_file;
		QDateTime m_start_time;
		
	public:
		FileScanStartedEvent(const QString &_file, const QDateTime &_start_time) : 
				ScanEvent(ScanEvent::FileScanStarted, 1000), m_file(_file), m_start_time(_start_time) {}
		~FileScanStartedEvent() {}
		
		QString file() const { return m_file; }
		QDateTime startTime() const { return m_start_time; }
	};
	
	class FileScanCompletedEvent : public ScanEvent
	{
	private:
		QString m_file;
		qint32 m_result;
		QDateTime m_start_time;
		QDateTime m_end_time;
		QString m_virname;
		
	public:
		FileScanCompletedEvent(const QString &_file, qint32 _result, const QDateTime &_start_time, 
							   const QDateTime &_end_time, const QString &_virname) : 
				ScanEvent(ScanEvent::FileScanCompleted, 1000), m_file(_file), m_result(_result), 
				m_start_time(_start_time), m_end_time(_end_time), m_virname(_virname) {}
		~FileScanCompletedEvent() {}
		
		QString file() const { return m_file; }
		qint32 result() const { return m_result; }
		QDateTime startTime() const { return m_start_time; }
		QDateTime endTime() const { return m_end_time; }
		QString virname() const { return m_virname; }
	};
	
	class ProcScanStartedEvent : public ScanEvent
	{
	private:
		QString m_name;
		Q_PID m_pid;
		QDateTime m_start_time;
		
	public:
		ProcScanStartedEvent(const QString &_name, Q_PID _pid, const QDateTime &_start_time) : 
				ScanEvent(ScanEvent::ProcScanStarted, 1000), m_name(_name), m_pid(_pid), m_start_time(_start_time) {}
		~ProcScanStartedEvent() {}
		
		QString name() const { return m_name; }
		Q_PID pid() const { return m_pid; }
		QDateTime startTime() const { return m_start_time; }
	};
	
	class ProcScanCompletedEvent : public ScanEvent
	{
	private:
		QString m_name;
		Q_PID m_pid;
		qint32 m_result;
		QDateTime m_start_time;
		QDateTime m_end_time;
		QString m_virname;
		
	public:
		ProcScanCompletedEvent(const QString &_name, Q_PID _pid, qint32 _result, const QDateTime &_start_time, 
							   const QDateTime &_end_time, const QString &_virname) : 
				ScanEvent(ScanEvent::ProcScanCompleted, 1000), m_name(_name), m_pid(_pid), m_result(_result), 
				m_start_time(_start_time), m_end_time(_end_time), m_virname(_virname) {}
		~ProcScanCompletedEvent() {}
		
		QString name() const { return m_name; }
		Q_PID pid() const { return m_pid; }
		qint32 status() const { return m_result; }
		QDateTime startTime() const { return m_start_time; }
		QDateTime endTime() const { return m_end_time; }
		QString virname() const { return m_virname; }
	};
	
	class DirScanStartedEvent : public ScanEvent
	{
	private:
		QString m_dir;
		QDateTime m_start_time;
		
	public:
		DirScanStartedEvent(const QString &_dir, const QDateTime &_start_time) : 
				ScanEvent(ScanEvent::DirScanStarted, 1000), m_dir(_dir), m_start_time(_start_time) {}
		~DirScanStartedEvent() {}
		
		QString dir() const { return m_dir; }
		QDateTime startTime() const { return m_start_time; }
	};
	
	class DirScanCompletedEvent : public ScanEvent
	{
	private:
		QString m_dir;
		QDateTime m_start_time;
		QDateTime m_end_time;
		
	public:
		DirScanCompletedEvent(const QString &_dir, const QDateTime &_start_time, const QDateTime &_end_time) : 
				ScanEvent(ScanEvent::DirScanCompleted, 500), m_dir(_dir), m_start_time(_start_time),
				m_end_time(_end_time) {}
		~DirScanCompletedEvent() {}
		
		QString dir() const { return m_dir; }
		QDateTime startTime() const { return m_start_time; }
		QDateTime endTime() const { return m_end_time; }
	};
	
	class MemScanStartedEvent : public ScanEvent
	{
	private:
		QDateTime m_start_time;
		
	public:
		MemScanStartedEvent(const QDateTime &_start_time) : 
				ScanEvent(ScanEvent::MemScanStarted, 1000), m_start_time(_start_time) {}
		~MemScanStartedEvent() {}
		
		QDateTime startTime() const { return m_start_time; }
	};
	
	class MemScanCompletedEvent : public ScanEvent
	{
	private:
		QDateTime m_start_time;
		QDateTime m_end_time;
		
	public:
		MemScanCompletedEvent(const QString &_dir, const QDateTime &_start_time, const QDateTime &_end_time) : 
				ScanEvent(ScanEvent::MemScanCompleted, 500), m_start_time(_start_time),
				m_end_time(_end_time) {}
		~MemScanCompletedEvent() {}
		
		QDateTime startTime() const { return m_start_time; }
		QDateTime endTime() const { return m_end_time; }
	};
}
#endif
