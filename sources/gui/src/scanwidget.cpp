
#include <QDebug>
#include <manager.h>

#include "scanwidget.h"

ScanWidget::ScanWidget(Manager *_man, QWidget *_par) : QWidget(_par), m_ui(), m_started(false), m_paused(false)
{
	m_ui.setupUi(this);
// 	connect(m_ui.m_start_button, SIGNAL(clicked()), this, SIGNAL(startFullScanSignal()));
	connect(m_ui.m_start_button, SIGNAL(clicked()), this, SLOT(startClickedSlot()));
	connect(m_ui.m_stop_button, SIGNAL(clicked()), this, SLOT(stopClickedSlot()));
	connect(_man, SIGNAL(memScanStartedSignal()), this, SLOT(memScanStartedSlot()));
	connect(_man, SIGNAL(memScanCompletedSignal()), this, SLOT(memScanCompletedSlot()));
	connect(_man, SIGNAL(dirScanStartedSignal()), this, SLOT(dirScanStartedSlot()));
	connect(_man, SIGNAL(dirScanCompletedSignal()), this, SLOT(dirScanCompletedSlot()));
// 	connect(_man, SIGNAL(scanStoppedSignal()), this, SLOT(scanStoppedSlot()));
	
	connect(this, SIGNAL(startFullScanSignal()), _man, SLOT(fullScan()));
	connect(this, SIGNAL(stopFullScanSignal()), _man, SLOT(stop()));
	connect(this, SIGNAL(pauseSignal()), _man, SLOT(pauseSlot()));
	connect(this, SIGNAL(resumeSignal()), _man, SLOT(resumeSlot()));
}

void ScanWidget::memScanStartedSlot()
{
	m_ui.m_scanobj_label -> setText(tr("Memory scan started"));
// 	scanStartedSlot();
}

void ScanWidget::memScanCompletedSlot()
{
	m_ui.m_scanobj_label -> setText(tr("Memory scan completed"));
}

void ScanWidget::dirScanStartedSlot()
{
	m_ui.m_scanobj_label -> setText(tr("Directory scan started"));
// 	scanStartedSlot();
}

void ScanWidget::dirScanCompletedSlot()
{
	m_ui.m_scanobj_label -> setText(tr("Directory scan completed"));
}

void ScanWidget::scanStartedSlot()
{
	m_started = true;
	m_ui.m_start_button -> setText("Stop");
	m_ui.m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
}

void ScanWidget::scanStoppedSlot()
{
	m_started = false;
	m_ui.m_start_button -> setText("Start");
	m_ui.m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	m_ui.m_scanobj_label -> setText(tr("Scan was stopped by user"));
	Q_EMIT showStatisticSignal();
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
		m_ui.m_start_button -> setText("Resume");
		m_ui.m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	}
	else
	{
		m_ui.m_start_button -> setText("Pause");
		m_ui.m_start_button -> setIcon(QIcon(":/images/images/pause.png"));
	}
}

void ScanWidget::stopClickedSlot()
{
	if(!m_started)
		return;
	Q_EMIT stopFullScanSignal();
	m_started = false;
	m_ui.m_start_button -> setText("Start");
	m_ui.m_start_button -> setIcon(QIcon(":/images/images/play.png"));
	m_ui.m_scanobj_label -> setText(tr("Scan was stopped by user"));
}
