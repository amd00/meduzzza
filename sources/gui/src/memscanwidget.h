
#ifndef _MEMSCANWIDGET_H_
#define _MEMSCANWIDGET_H_

#include "meduzzzacommonwidget.h"

namespace Ui
{
	class MemScanWidget;
}

namespace Meduzzza
{
	class MemScanWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::MemScanWidget *m_ui;
		bool m_started;
		bool m_paused;
		
	public:
		MemScanWidget(Manager *_man, MainWindow *_med);
		~MemScanWidget();
		
		QString text() const { return tr("Memory scan"); }
		
	protected Q_SLOTS:
		void startClickedSlot();
		void stopClickedSlot();
		
		void procScanStartedSlot(const QString &_name, Q_PID _pid);
		void procScanCompletedSlot(const QString &_name, Q_PID _pid);
		void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QString &_virname);
		
		void memScanStartedSlot();
		void memScanCompletedSlot();
		
		void stoppedSlot();
		void pausedSlot();
		void resumedSlot();
	};
}

#endif
