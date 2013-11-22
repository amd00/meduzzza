
#include <QDebug>
#include <manager.h>

#include <ui_scanwidget.h>

#include "scanwidget.h"
#include "scanreswidget.h"
#include "mainwindow.h"

namespace Meduzzza
{

	ScanWidget::ScanWidget(Manager *_man, MainWindow *_med, QWidget *_parent) : QWidget(_parent), 
		m_ui(new Ui::ScanWidget),  m_man(_man), m_med(_med), m_started(false), m_paused(false)
	{
		m_ui -> setupUi(this);
		connect(m_ui -> m_start_button, SIGNAL(clicked()), this, SLOT(startClickedSlot()));
		connect(m_ui -> m_stop_button, SIGNAL(clicked()), this, SLOT(stopClickedSlot()));
		connect(_man, SIGNAL(memScanStartedSignal()), this, SLOT(memScanStartedSlot()));
		connect(_man, SIGNAL(memScanCompletedSignal()), this, SLOT(memScanCompletedSlot()));
		connect(_man, SIGNAL(dirScanStartedSignal(const QString&)), this, SLOT(dirScanStartedSlot(const QString&)));
		connect(_man, SIGNAL(dirScanCompletedSignal(const QString&)), this, SLOT(dirScanCompletedSlot(const QString&)));
		
		connect(this, SIGNAL(startFullScanSignal()), _man, SLOT(fullScan()));
// 		connect(this, SIGNAL(stopFullScanSignal()), _man, SLOT(stop()));
// 		connect(this, SIGNAL(pauseSignal()), _man, SLOT(pauseSlot()));
// 		connect(this, SIGNAL(resumeSignal()), _man, SLOT(resumeSlot()));
	}

	ScanWidget::~ScanWidget() { delete m_ui; }

	void ScanWidget::memScanStartedSlot()
	{
		m_ui -> m_scanobj_label -> setText(tr("Memory scan started"));
	// 	scanStartedSlot();
	}

	void ScanWidget::memScanCompletedSlot()
	{
		m_ui -> m_scanobj_label -> setText(tr("Memory scan completed"));
	}

	void ScanWidget::dirScanStartedSlot(const QString &_dir)
	{
		m_ui -> m_scanobj_label -> setText(tr("Directory scan started"));
	// 	scanStartedSlot();
	}

	void ScanWidget::dirScanCompletedSlot(const QString &_dir)
	{
		m_ui -> m_scanobj_label -> setText(tr("Directory scan completed"));
	}

	void ScanWidget::scanStartedSlot()
	{
		m_started = true;
		m_ui -> m_start_button -> setText("Stop");
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}

	void ScanWidget::scanStoppedSlot()
	{
		m_started = false;
		m_ui -> m_start_button -> setText("Start");
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		m_ui -> m_scanobj_label -> setText(tr("Scan was stopped by user"));
	// 	Q_EMIT showStatisticSignal();
		ScanResWidget *srw(new ScanResWidget(m_man -> filesCount(), 
				m_man -> fileVirusesCount(), m_man -> quarantined(), NULL));
		m_med -> showWidget(srw);
	}

	void ScanWidget::startClickedSlot()
	{
		if(!m_started)
		{
			Q_EMIT startFullScanSignal();
			m_started = true;
		}
		else
		{
			Q_EMIT (m_paused ? resumeSignal() : pauseSignal());
			m_paused = !m_paused;
		}
		if(m_paused)
		{
			m_ui -> m_start_button -> setText("Resume");
			m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		}
		else
		{
			m_ui -> m_start_button -> setText("Pause");
			m_ui -> m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
		}
	}

	void ScanWidget::stopClickedSlot()
	{
		if(!m_started)
			return;
		Q_EMIT stopFullScanSignal();
		m_started = false;
		m_ui -> m_start_button -> setText("Start");
		m_ui -> m_start_button -> setIcon(QIcon(":/images/images/play.png"));
		m_ui -> m_scanobj_label -> setText(tr("Scan was stopped by user"));
	}

}
