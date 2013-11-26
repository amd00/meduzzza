
#ifndef _SCANWIDGET_H_
#define _SCANWIDGET_H_

#include "meduzzzacommonwidget.h"

namespace Ui
{
	class ScanWidget;
}

namespace Meduzzza
{
	class ScanWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::ScanWidget *m_ui;
		
	public:
		ScanWidget(MainWindow *_med);
		~ScanWidget();
		QString text() const { return tr("Full scan"); }
		
	private:
		void fileScanStarted(const QString &_file) {}
		void fileScanCompleted(const QString &_file) {}
		void fileVirusDetected(const QString &_file, const QString &_virname) {}
		
		void procScanStarted(const QString &_name, Q_PID _pid) {}
		void procScanCompleted(const QString &_name, Q_PID _pid) {}
		void procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname) {}
		
		void memScanStarted();
		void memScanCompleted();
		
		void dirScanStarted(const QString &_dir);
		void dirScanCompleted(const QString &_dir);
		
		void stopped();
		void paused();
		void resumed();
		
		void fullScanStarted(const QDateTime &_time);
		void fullScanCompleted(const QDateTime &_time);
		
	private Q_SLOTS:
		void startClickedSlot();
		void stopClickedSlot();
	};
}
#endif
