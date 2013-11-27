
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
	
	class MeduzzzaScanModel;

	class MemScanWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::MemScanWidget *m_ui;
		MeduzzzaScanModel *m_mod;
		
	public:
		MemScanWidget(MainWindow *_med);
		~MemScanWidget();
		
		QString text() const { return tr("Memory scan"); }
		
	protected:
		void fileScanStarted(const QString &_file) {}
		void fileScanCompleted(const QString &_file) {}
		void fileVirusDetected(const QString &_file, const QString &_virname) {}
		
		void procScanStarted(const QString &_name, Q_PID _pid);
		void procScanCompleted(const QString &_name, Q_PID _pid);
		void procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname);
		
		void dirScanStarted(const QString &_dir);
		void dirScanCompleted(const QString &_dir);
		
		void memScanStarted();
		void memScanCompleted();
		
		void fullScanStarted(const QDateTime &_time);
		void fullScanCompleted(const QDateTime &_time);
		
		void stopped();
		void paused();
		void resumed();
		
	protected Q_SLOTS:
		void startClickedSlot();
		void stopClickedSlot();
		
		void rowsInsertedSlot(const QModelIndex &_par, qint32 _start, qint32 _end);
	};
}

#endif
