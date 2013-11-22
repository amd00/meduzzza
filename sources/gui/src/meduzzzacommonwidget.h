
#ifndef _MEDUZZZACOMMONWIDGET_H_
#define _MEDUZZZACOMMONWIDGET_H_

#include <QWidget>

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
		virtual void fileScanStartedSlot(const QString &_file) = 0;
		virtual void fileScanCompletedSlot(const QString &_file) = 0;
		virtual void fileVirusDetectedSlot(const QString &_file, const QString &_virname) = 0;
		
		virtual void dirScanStartedSlot(const QString &_dir) = 0;
		virtual void dirScanCompletedSlot(const QString &_dir) = 0;
		
		virtual void stoppedSlot() = 0;
		virtual void pausedSlot() = 0;
		virtual void resumedSlot() = 0;
		
	};
}
#endif
