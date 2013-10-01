
#ifndef _SCANWIDGET_H_
#define _SCANWIDGET_H_

#include <ui_scanwidget.h>

class Manager;

class ScanWidget : public QWidget
{
	Q_OBJECT
	
private:
	Ui::ScanWidget m_ui;
	bool m_started;
	bool m_paused;
	
public:
	ScanWidget(Manager *_man, QWidget *_par);
	~ScanWidget() {}
	QString text() const { return tr("Full scan"); }
	
private Q_SLOTS:
	void memScanStartedSlot();
	void memScanCompletedSlot();
	void dirScanStartedSlot();
	void dirScanCompletedSlot();
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

#endif
