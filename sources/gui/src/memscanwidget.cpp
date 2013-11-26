
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
			MeduzzzaCommonWidget(_med), m_ui(new Ui::MemScanWidget), m_mod(),
			m_started(false), m_paused(false)
	{
		m_ui -> setupUi(this);
		m_ui -> m_scan_view -> setModel(&m_mod);
	}

	MemScanWidget::~MemScanWidget() { delete m_ui; }

	void MemScanWidget::startClickedSlot()
	{
		if(!m_started)
		{
			((MeduzzzaScanModel*)m_mod.sourceModel()) -> clear();
			m_man -> scanMemory();
		}
		else if(!m_paused)
			m_man -> pause();
		else
			m_man -> resume();
	}
	
	void MemScanWidget::stopClickedSlot()
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_man -> stop();
	}
	
	void MemScanWidget::procScanStartedSlot(const QString &_name, Q_PID _pid)
	{
	
	}

	void MemScanWidget::procScanCompletedSlot(const QString &_name, Q_PID _pid)
	{
	}

	void MemScanWidget::procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QString &_virus)
	{
	}

	void MemScanWidget::memScanStartedSlot()
	{
		m_started = true;
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}

	void MemScanWidget::memScanCompletedSlot()
	{
		m_started = false;
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void MemScanWidget::stoppedSlot()
	{
		m_started = false;
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		QApplication::restoreOverrideCursor();
	}

	void MemScanWidget::pausedSlot()
	{
		m_paused = true;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void MemScanWidget::resumedSlot()
	{
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
	
}
