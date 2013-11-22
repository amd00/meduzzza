
#ifndef _DIRSCANWIDGET_H_
#define _DIRSCANWIDGET_H_

#include "meduzzzacommonwidget.h"

namespace Ui
{
	class DirScanWidget;
}

namespace Meduzzza
{
	class DirScanWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::DirScanWidget *m_ui;
		bool m_started;
		bool m_paused;
		
	public:
		DirScanWidget(Manager *_man, MainWindow *_med);
		~DirScanWidget();
		
		QString text() const { return tr("Directory scan"); }
		
	protected Q_SLOTS:
		void dirClickedSlot();
		void startClickedSlot();
		void stopClickedSlot();
		
		void fileScanStartedSlot(const QString &_file);
		void fileScanCompletedSlot(const QString &_file);
		void fileVirusDetectedSlot(const QString &_file, const QString &_virname);
		
		void dirScanStartedSlot(const QString &_dir);
		void dirScanCompletedSlot(const QString &_dir);
		
		void stoppedSlot();
		void pausedSlot();
		void resumedSlot();
	};
}

#endif
