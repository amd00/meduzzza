
#ifndef _UPDATEWIDGET_H_
#define _UPDATEWIDGET_H_

#include <QWidget>

class Manager;
class Meduzzza;
class QProgressBar;

namespace Ui
{
	class UpdateWidget;
}

typedef QSharedPointer<QLabel> LabelPtr;
typedef QSharedPointer<QProgressBar> ProgressBarPtr;

class UpdateWidget : public QWidget
{
	Q_OBJECT
	
private:
	Ui::UpdateWidget *m_ui;
	QMap<QString, QPair<LabelPtr, ProgressBarPtr> > m_items;
	Manager *m_man;
	Meduzzza *m_med;
	
public:
	explicit UpdateWidget(Manager *_man, Meduzzza *_med, QWidget *_parent = NULL);
	~UpdateWidget();
	QString text() { return tr("Update"); }
	
private Q_SLOTS:
	void downloadStartedSlot(const QString &_file);
	void downloadFinishedSlot(const QString &_file);
	void downloadProgressSlot(const QString &_file, qint64 _read, qint64 _total);
	void startUpdateSlot();
	void updateCompletedSlot();
};

#endif
