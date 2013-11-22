
#ifndef _SCANWIDGET_H_
#define _SCANWIDGET_H_

#include <QWidget>

namespace Ui
{
	class ScanWidget;
}

namespace Meduzzza
{
	class Manager;
	class MainWindow;
	
	class ScanWidget : public QWidget
	{
		Q_OBJECT
		
	private:
		Ui::ScanWidget *m_ui;
		Manager *m_man;
		MainWindow *m_med;
		bool m_started;
		bool m_paused;
		
	public:
		ScanWidget(Manager *_man, MainWindow *_med, QWidget *_parent = NULL);
		~ScanWidget();
		QString text() const { return tr("Full scan"); }
		
	private Q_SLOTS:
		void memScanStartedSlot();
		void memScanCompletedSlot();
		void dirScanStartedSlot(const QString &_dir);
		void dirScanCompletedSlot(const QString &_dir);
		void scanStartedSlot();
		void scanStoppedSlot();
		void startClickedSlot();
		void stopClickedSlot();
		
	Q_SIGNALS:
		void startFullScanSignal();
		void stopFullScanSignal();
		void pauseSignal();
		void resumeSignal();
		void showStatisticSignal();
	};
}
#endif
