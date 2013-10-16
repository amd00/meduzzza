
#ifndef _DIRSCANWIDGET_H_
#define _DIRSCANWIDGET_H_

#include <QWidget>

namespace Ui
{
	class DirScanWidget;
}

class Manager;
class Meduzzza;

class DirScanWidget : public QWidget
{
	Q_OBJECT
	
private:
	Ui::DirScanWidget *m_ui;
	Manager *m_man;
	Meduzzza *m_med;
	
public:
	DirScanWidget(Manager *_man, Meduzzza *_med, QWidget *_parent = NULL);
	~DirScanWidget();
	
	QString text() const { return tr("Directory scan"); }
	
private Q_SLOTS:
	void dirClickedSlot();
	void startClickedSlot();
	void dirScanCompletedSlot();
	void fileScanStartedSlot(const QString &_file);
	void fileScanCompletedSlot(const QString &_file);
	void fileVirusDetectedSlot(const QString &_file, const QString &_virus);
};

#endif
