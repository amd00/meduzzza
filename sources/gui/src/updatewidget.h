
#ifndef _UPDATEWIDGET_H_
#define _UPDATEWIDGET_H_

#include <QWidget>

class QProgressBar;

namespace Ui
{
	class UpdateWidget;
}

namespace Meduzzza
{
	class Manager;
	class MainWindow;
	
	class UpdateWidget : public QWidget
	{
		Q_OBJECT
		
	private:
		Ui::UpdateWidget *m_ui;
		QMap<QString, QPair<QLabel*, QProgressBar*> > m_items;
		Manager *m_man;
		MainWindow *m_med;
		
	public:
		explicit UpdateWidget(Manager *_man, MainWindow *_med, QWidget *_parent = NULL);
		~UpdateWidget();
		QString text() { return tr("Update"); }
		
	private Q_SLOTS:
		void downloadStartedSlot(const QString &_file);
		void downloadFinishedSlot(const QString &_file);
		void downloadProgressSlot(const QString &_file, qint64 _read, qint64 _total);
		void startUpdateSlot();
		void updateCompletedSlot();
	};
}
#endif
