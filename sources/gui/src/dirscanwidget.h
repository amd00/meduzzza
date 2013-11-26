
#ifndef _DIRSCANWIDGET_H_
#define _DIRSCANWIDGET_H_

#include <QSortFilterProxyModel>

#include "meduzzzacommonwidget.h"

namespace Ui
{
	class DirScanWidget;
}

namespace Meduzzza
{
	
	class DirScanModel : public QSortFilterProxyModel
	{
		Q_OBJECT
		
	public:
		DirScanModel();
		~DirScanModel() {}
	
	protected:
		bool filterAcceptsRow(qint32 _row, const QModelIndex &_parent) const;
	};

//==============================================================================//	
	
	class DirScanWidget : public MeduzzzaCommonWidget
	{
		Q_OBJECT
		
	private:
		Ui::DirScanWidget *m_ui;
		DirScanModel m_mod;
		
	public:
		DirScanWidget(MainWindow *_med);
		~DirScanWidget();
		
		QString text() const { return tr("Directory scan"); }
		
	private:
		void fileScanStarted(const QString &_file);
		void fileScanCompleted(const QString &_file);
		void fileVirusDetected(const QString &_file, const QString &_virname);
		
		void procScanStarted(const QString &_name, Q_PID _pid) {}
		void procScanCompleted(const QString &_name, Q_PID _pid) {}
		void procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname) {}
		
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
		void dirClickedSlot();
		void startClickedSlot();
		void stopClickedSlot();
		
		void rowsInsertedSlot(const QModelIndex &_par, qint32 _start, qint32 _end);
	};
}

#endif
