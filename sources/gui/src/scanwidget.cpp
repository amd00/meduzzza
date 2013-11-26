
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

	void ScanWidget::memScanStarted()
	{
		m_ui -> m_scanobj_label -> setText(tr("Memory scanning..."));
	}
	
	void ScanWidget::memScanCompleted()
	{
		
	}
	
	void ScanWidget::dirScanStarted(const QString &_dir)
	{
		m_ui -> m_scanobj_label -> setText(tr("Filesystem scanning..."));
	}
	
	void ScanWidget::dirScanCompleted(const QString &_dir)
	{
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
