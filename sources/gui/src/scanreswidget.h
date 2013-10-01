
#ifndef _SCANRESWIDGET_H_
#define _SCANRESWIDGET_H_

#include <ui_scanreswidget.h>

class ScanResWidget : public QWidget
{
	Q_OBJECT
	
private:
	Ui::ScanResWidget m_ui;
	
public:
	ScanResWidget(quint64 _files, quint64 _infected, quint64 _quarantined, QWidget *_par);
	~ScanResWidget() {}
};

#endif