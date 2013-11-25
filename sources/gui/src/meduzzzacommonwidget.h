
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
		
	public:
		MeduzzzaCommonWidget(Manager *_man, MainWindow *_mw);
		virtual ~MeduzzzaCommonWidget() {}
		
	protected Q_SLOTS:
		virtual void fileScanStartedSlot(const QString &_file) {}
		virtual void fileScanCompletedSlot(const QString &_file) {}
		virtual void fileVirusDetectedSlot(const QString &_file, const QString &_virname) {}
		
		virtual void procScanStartedSlot(const QString &_name, Q_PID _pid) {}
		virtual void procScanCompletedSlot(const QString &_name, Q_PID _pid) {}
		virtual void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QString &_virname) {}
		
		virtual void dirScanStartedSlot(const QString &_dir) {}
		virtual void dirScanCompletedSlot(const QString &_dir) {}
		
		virtual void memScanStartedSlot() {}
		virtual void memScanCompletedSlot() {}
		
		virtual void stoppedSlot() {}
		virtual void pausedSlot() {}
		virtual void resumedSlot() {}
		
	};
}
#endif
