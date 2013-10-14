
#ifndef _SCANWIDGET_H_
#define _SCANWIDGET_H_

#include <QWidget>

class Manager;
class Meduzzza;

namespace Ui
{
	class ScanWidget;
}

class ScanWidget : public QWidget
{
	Q_OBJECT
	
private:
	Ui::ScanWidget *m_ui;
	Manager *m_man;
	Meduzzza *m_med;
	bool m_started;
	bool m_paused;
	
public:
	ScanWidget(Manager *_man, Meduzzza *_med, QWidget *_parent = NULL);
	~ScanWidget();
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
