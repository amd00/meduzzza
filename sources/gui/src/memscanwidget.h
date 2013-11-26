
#ifndef _MEMSCANWIDGET_H_
#define _MEMSCANWIDGET_H_

#include <QSortFilterProxyModel>

#include "meduzzzacommonwidget.h"

namespace Ui
{
	class MemScanWidget;
}

namespace Meduzzza
{
	class MemScanModel : public QSortFilterProxyModel
	{
		Q_OBJECT
		
	public:
		MemScanModel();
		~MemScanModel() {}
	
	protected:
		bool filterAcceptsRow(qint32 _row, const QModelIndex &_parent) const;
	};

//==============================================================================//

	class MemScanWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::MemScanWidget *m_ui;
		MemScanModel m_mod;
		bool m_started;
		bool m_paused;
		
	public:
		MemScanWidget(MainWindow *_med);
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
