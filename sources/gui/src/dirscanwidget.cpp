
#include <QFileDialog>

#include <manager.h>

#include <ui_dirscanwidget.h>

#include "mainwindow.h"
#include "dirscanwidget.h"
#include "meduzzzascanmodel.h"
#include "scanresdialog.h"

namespace Meduzzza
{

	DirScanWidget::DirScanWidget(MainWindow *_med) : 
			MeduzzzaCommonWidget(_med), m_ui(new Ui::DirScanWidget), m_mod(new MeduzzzaScanModel(false))
	{
		m_ui -> setupUi(this);
		m_ui -> m_scan_view -> setModel(m_mod);
		m_ui -> m_scan_view -> setColumnHidden(MeduzzzaScanModel::Pid, true);
	}

	DirScanWidget::~DirScanWidget() { delete m_ui; }

	void DirScanWidget::fileScanStarted(const QString &_file)
	{
	}

	void DirScanWidget::fileScanCompleted(const QString &_file)
	{
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1);
	}

	void DirScanWidget::fileVirusDetected(const QString &_file, const QString &_virus)
	{
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1);
	}

	void DirScanWidget::dirScanStarted(const QString &_dir)
	{
		m_ui -> m_progress -> setValue(0);
		m_mod -> clear();
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}

	void DirScanWidget::dirScanCompleted(const QString &_dir)
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		ScanResDialog res;
		res.exec();
	}
	
	void DirScanWidget::memScanStarted()
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}
	
	void DirScanWidget::memScanCompleted()
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}
		
	void DirScanWidget::fullScanStarted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(true);
		m_ui -> m_stop_button -> setDisabled(true);
	}
	
	void DirScanWidget::fullScanCompleted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
	}

	void DirScanWidget::stopped()
	{
		m_ui -> m_start_button -> setDisabled(false);
		m_ui -> m_stop_button -> setDisabled(false);
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		QApplication::restoreOverrideCursor();
	}

	void DirScanWidget::paused()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}

	void DirScanWidget::resumed()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
	
	void DirScanWidget::filesFound(quint64 _count)
	{
		m_ui -> m_progress -> setMaximum(_count);
	}
	
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
			m_man -> scanDir(m_ui -> m_dir_edit -> text());
		else if(!m_paused)
			m_man -> pause();
		else
			m_man -> resume();
	}
	
	void DirScanWidget::stopClickedSlot()
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_ui -> m_stop_button -> setDisabled(true);
		m_man -> stop();
	}

	
}
