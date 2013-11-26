
#ifndef _MEDUZZZA_H_
#define _MEDUZZZA_H_

#include <QWidget>

namespace Ui
{
	class MainWindow;
}

namespace Meduzzza
{
	class Manager;
	class MainMenuModel;
	
	class MainWindow : public QWidget
	{
		Q_OBJECT

	private:
		Ui::MainWindow *m_ui;
		MainMenuModel *m_menu;
		Manager *m_man;
		
	public:
		MainWindow();
		~MainWindow();
		
		void showWidget(QWidget *_w);
		
	protected:
		void closeEvent(QCloseEvent *_event);
		
	private Q_SLOTS:
		void menuSelectedSlot(const QModelIndex &_current, const QModelIndex &_previous);
	};
}
#endif
