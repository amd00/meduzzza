
#include <manager.h>

#include <ui_scanwidget.h>

#include "scanwidget.h"
#include "scanreswidget.h"
#include "mainwindow.h"

namespace Meduzzza
{

	ScanWidget::ScanWidget(MainWindow *_med) : MeduzzzaCommonWidget(_med), 
		m_ui(new Ui::ScanWidget)
	{
		m_ui -> setupUi(this);
	}

	ScanWidget::~ScanWidget() { delete m_ui; }

	void ScanWidget::fileScanCompleted(const QString &_file) 
	{ 
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1); 
	}
	
	void ScanWidget::fileVirusDetected(const QString &_file, const QString &_virname) 
	{ 
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1); 
	}
	
	void ScanWidget::procScanCompleted(const QString &_name, Q_PID _pid) 
	{ 
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1); 
	}
	
	void ScanWidget::procVirusDetected(const QString &_name, Q_PID _pid, const QString &_virname) 
	{ 
		m_ui -> m_progress -> setValue(m_ui -> m_progress -> value() + 1); 
	}
	
	void ScanWidget::memScanStarted()
	{
		m_ui -> m_scanobj_label -> setText(tr("Memory scanning..."));
	}
	
	void ScanWidget::memScanCompleted()
	{
		m_ui -> m_progress -> setValue(0);
	}
	
	void ScanWidget::dirScanStarted(const QString &_dir)
	{
		m_ui -> m_scanobj_label -> setText(tr("Filesystem scanning..."));
	}
	
	void ScanWidget::dirScanCompleted(const QString &_dir)
	{
		m_ui -> m_progress -> setValue(0);
	}
	
	void ScanWidget::stopped()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		QApplication::restoreOverrideCursor();
	}
	
	void ScanWidget::paused()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}
	
	void ScanWidget::resumed()
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
	
	void ScanWidget::filesFound(quint64 _count) 
	{ 
		m_ui -> m_progress -> setMaximum(_count); 
	}
		
	void ScanWidget::procsFound(quint64 _count) 
	{ 
		m_ui -> m_progress -> setMaximum(_count); 
	}
	
	void ScanWidget::fullScanStarted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
	
	void ScanWidget::fullScanCompleted(const QDateTime &_time)
	{
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}
	
	void ScanWidget::startClickedSlot()
	{
		if(!m_started)
			m_man -> fullScan();
		else if(!m_paused)
			m_man -> pause();
		else
			m_man -> resume();
	}

	void ScanWidget::stopClickedSlot()
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		m_man -> stop();
	}

}
