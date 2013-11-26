
#include <QFileDialog>

#include <manager.h>

#include <ui_dirscanwidget.h>

#include "mainwindow.h"
#include "dirscanwidget.h"
#include "meduzzzascanmodel.h"

namespace Meduzzza
{
	DirScanModel::DirScanModel() : QSortFilterProxyModel()
	{
		MeduzzzaScanModel *mod = MeduzzzaScanModel::get();
		setSourceModel(mod);
	}

	bool DirScanModel::filterAcceptsRow(qint32 _row, const QModelIndex &_parent) const
	{
		QModelIndex ind = sourceModel() -> index(_row, MeduzzzaScanModel::Pid);
		if(ind.data().isNull())
			return true;
		return false;
	}



	DirScanWidget::DirScanWidget(MainWindow *_med) : 
			MeduzzzaCommonWidget(_med), m_ui(new Ui::DirScanWidget), m_mod(),
			m_started(false), m_paused(false)
	{
		m_ui -> setupUi(this);
		m_ui -> m_scan_view -> setModel(&m_mod);
		m_ui -> m_scan_view -> setColumnHidden(MeduzzzaScanModel::Pid, true);
	}

	DirScanWidget::~DirScanWidget() { delete m_ui; }

	void DirScanWidget::dirClickedSlot()
	{
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if(!dir.isEmpty())
			m_ui -> m_dir_edit -> setText(dir);
	}

	void DirScanWidget::startClickedSlot()
	{
		if(!m_started)
		{
			((MeduzzzaScanModel*)m_mod.sourceModel()) -> clear();
			m_man -> scanDir(m_ui -> m_dir_edit -> text());
		}
		else if(!m_paused)
			m_man -> pause();
		else
			m_man -> resume();
	}
	
	void DirScanWidget::stopClickedSlot()
	{
		m_man -> stop();
	}
	
	void DirScanWidget::fileScanStartedSlot(const QString &_file)
	{

	}

	void DirScanWidget::fileScanCompletedSlot(const QString &_file)
	{
	}

	void DirScanWidget::fileVirusDetectedSlot(const QString &_file, const QString &_virus)
	{
	}

	void DirScanWidget::dirScanStartedSlot(const QString &_dir)
	{
		m_started = true;
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}

	void DirScanWidget::dirScanCompletedSlot(const QString &_dir)
	{
		m_started = false;
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void DirScanWidget::stoppedSlot()
	{
		m_started = false;
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void DirScanWidget::pausedSlot()
	{
		m_paused = true;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void DirScanWidget::resumedSlot()
	{
		m_paused = false;
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
}
