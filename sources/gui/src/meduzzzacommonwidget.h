
#ifndef _MEDUZZZACOMMONWIDGET_H_
#define _MEDUZZZACOMMONWIDGET_H_

#include <QWidget>
#include <QProcess>

namespace Meduzzza
{
	class Manager;
	class MainWindow;
	
	class MeduzzzaCommonWidget : public QWidget
	{
		Q_OBJECT
		
	protected:
		Manager *m_man;
		MainWindow *m_mw;
		bool m_started;
		bool m_paused;
		
	public:
		MeduzzzaCommonWidget(MainWindow *_mw);
		virtual ~MeduzzzaCommonWidget() {}
		
	protected:
		virtual void fileScanStarted(const QString &_file) = 0;
		virtual void fileScanCompleted(const QString &_file) = 0;
		virtual void fileVirusDetected(const QString &_file, const QString &_virname) = 0;
		
		virtual void procScanStarted(const QString &_name, Q_PID _pid) = 0;
		virtual void procScanCompleted(const QString &_name, Q_PID _pid) = 0;
		virtual void procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname) = 0;
		virtual void procScanError(const QString &_name, Q_PID _pid, const QString &_error) = 0;
		
		virtual void dirScanStarted(const QString &_dir) = 0;
		virtual void dirScanCompleted(const QString &_dir) = 0;
		
		virtual void memScanStarted() = 0;
		virtual void memScanCompleted() = 0;
		
		virtual void fullScanStarted(const QDateTime &_time) = 0;
		virtual void fullScanCompleted(const QDateTime &_time) = 0;
		
		virtual void stopped() = 0;
		virtual void paused() = 0;
		virtual void resumed() = 0;
		
		virtual void filesFound(quint64 _count) = 0;
		virtual void procsFound(quint64 _count) = 0;
		
	private Q_SLOTS:
		void fileScanStartedSlot(const QString &_file, const QDateTime &_time_start) { fileScanStarted(_file); }
		void fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end) { fileScanCompleted(_file); }
		void fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virname) { fileVirusDetected(_file, _virname); }
		
		void procScanStartedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start) { procScanStarted(_name, _pid); }
		void procScanCompletedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end) { procScanCompleted(_name, _pid); }
		void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virname) { procVirusDetected(_name, _pid, _virname); }
		void procScanErrorSlot(const QString &_name, Q_PID _pid, const QString &_error) { procScanError(_name, _pid, _error); }
		
		void dirScanStartedSlot(const QString &_dir, const QDateTime &_time_start);
		void dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end);
		
		void memScanStartedSlot(const QDateTime &_time_start);
		void memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void fullScanStartedSlot(const QDateTime &_time_start);
		void fullScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end);
		
		void stoppedSlot();
		void pausedSlot();
		void resumedSlot();
		
		void filesFoundSlot(quint64 _count) { filesFound(_count); }
		void procsFoundSlot(quint64 _count) { procsFound(_count); }
	};
}
#endif
