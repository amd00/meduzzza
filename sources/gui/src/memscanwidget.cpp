
#include <manager.h>

#include <ui_memscanwidget.h>

#include "mainwindow.h"
#include "memscanwidget.h"
#include "meduzzzascanmodel.h"

namespace Meduzzza
{
	
	MemScanModel::MemScanModel() : QSortFilterProxyModel()
	{
		MeduzzzaScanModel *mod = MeduzzzaScanModel::get();
		setSourceModel(mod);
	}

	bool MemScanModel::filterAcceptsRow(qint32 _row, const QModelIndex &_parent) const
	{
		QModelIndex ind = sourceModel() -> index(_row, MeduzzzaScanModel::Pid);
		if(ind.data().isNull())
			return false;
		return true;
	}
	
	

	MemScanWidget::MemScanWidget(MainWindow *_med) : 
			MeduzzzaCommonWidget(_med), m_ui(new Ui::MemScanWidget), m_mod()
	{
		m_ui -> setupUi(this);
		connect(&m_mod, SIGNAL(rowsInserted(const QModelIndex&, qint32, qint32)),
			this, SLOT(rowsInsertedSlot(const QModelIndex&, qint32, qint32)));
		m_ui -> m_scan_view -> setModel(&m_mod);
	}

	MemScanWidget::~MemScanWidget() { delete m_ui; }

	void MemScanWidget::startClickedSlot()
	{
		if(!m_started)
			m_man -> scanMemory();
		else if(!m_paused)
			m_man -> pause();
		else
			m_man -> resume();
	}
	
	void MemScanWidget::stopClickedSlot()
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_ui -> m_stop_button -> setDisabled(true);
		m_man -> stop();
	}
	
	void MemScanWidget::procScanStarted(const QString &_name, Q_PID _pid)
	{
	
	}

	void MemScanWidget::procScanCompleted(const QString &_name, Q_PID _pid)
	{
	}

	void MemScanWidget::procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virus)
	{
	}
	
	void MemScanWidget::dirScanStarted(const QString &_dir)
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}

	void MemScanWidget::dirScanCompleted(const QString &_dir)
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}

	void MemScanWidget::memScanStarted()
	{
		((MeduzzzaScanModel*)m_mod.sourceModel()) -> clear();
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}

	void MemScanWidget::memScanCompleted()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}
	
	void MemScanWidget::fullScanStarted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}
	
	void MemScanWidget::fullScanCompleted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}

	void MemScanWidget::stopped()
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		QApplication::restoreOverrideCursor();
	}

	void MemScanWidget::paused()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void MemScanWidget::resumed()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
	
	void MemScanWidget::rowsInsertedSlot(const QModelIndex &_par, qint32 _start, qint32 _end)
	{
		QModelIndex ind = m_mod.index(_end, 0, _par);
		m_ui -> m_scan_view -> scrollTo(ind);
	}
	
}
