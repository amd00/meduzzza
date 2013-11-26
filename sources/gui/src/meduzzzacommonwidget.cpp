
#include <manager.h>

#include "meduzzzacommonwidget.h"
#include "mainwindow.h"

namespace Meduzzza
{

	MeduzzzaCommonWidget::MeduzzzaCommonWidget(MainWindow *_mw) : QWidget(), m_man(Manager::get()), m_mw(_mw),
			m_started(false), m_paused(false)
	{
			connect(m_man, SIGNAL(fileScanStartedSignal(const QString&)), this, SLOT(fileScanStartedSlot(const QString&)));
			connect(m_man, SIGNAL(fileScanCompletedSignal(const QString&)), this, SLOT(fileScanCompletedSlot(const QString&)));
			connect(m_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QString&)), 
					this, SLOT(fileVirusDetectedSlot(const QString&, const QString&)));
			
			connect(m_man, SIGNAL(procScanStartedSignal(const QString&, Q_PID)), this, SLOT(procScanStartedSlot(const QString&, Q_PID)));
			connect(m_man, SIGNAL(procScanCompletedSignal(const QString&, Q_PID)), this, SLOT(procScanCompletedSlot(const QString&, Q_PID)));
			connect(m_man, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QString&)), 
					this, SLOT(procVirusDetectedSlot(const QString&, Q_PID, const QString&)));
			
			connect(m_man, SIGNAL(dirScanStartedSignal(const QString&)), this, SLOT(dirScanStartedSlot(const QString&)));
			connect(m_man, SIGNAL(dirScanCompletedSignal(const QString&)), this, SLOT(dirScanCompletedSlot(const QString&)));
			
			connect(m_man, SIGNAL(memScanStartedSignal()), this, SLOT(memScanStartedSlot()));
			connect(m_man, SIGNAL(memScanCompletedSignal()), this, SLOT(memScanCompletedSlot()));
			
			connect(m_man, SIGNAL(fullScanStartedSignal(const QDateTime&)), this, SLOT(fullScanStartedSlot(const QDateTime&)));
			connect(m_man, SIGNAL(fullScanCompletedSignal(const QDateTime&)), this, SLOT(fullScanCompletedSlot(const QDateTime&)));
			
			connect(m_man, SIGNAL(stoppedSignal()), this, SLOT(stoppedSlot()));
			connect(m_man, SIGNAL(pausedSignal()), this, SLOT(pausedSlot()));
			connect(m_man, SIGNAL(resumedSignal()), this, SLOT(resumedSlot()));
	}
	
	void MeduzzzaCommonWidget::dirScanStartedSlot(const QString &_dir) 
	{
		m_started = true;
		m_paused = false;
		dirScanStarted(_dir);
	}
	
	void MeduzzzaCommonWidget::dirScanCompletedSlot(const QString &_dir) 
	{
		m_started = false;
		m_paused = false;
		dirScanCompleted(_dir);
	}
	
	void MeduzzzaCommonWidget::memScanStartedSlot() 
	{
		m_started = true;
		m_paused = false;
		memScanStarted();
	}
	
	void MeduzzzaCommonWidget::memScanCompletedSlot() 
	{
		m_started = false;
		m_paused = false;
		memScanCompleted();
	}
	
	void MeduzzzaCommonWidget::fullScanStartedSlot(const QDateTime &_time) 
	{
		m_started = true;
		m_paused = false;
		fullScanStarted(_time);
	}
	
	void MeduzzzaCommonWidget::fullScanCompletedSlot(const QDateTime &_time) 
	{
		m_started = false;
		m_paused = false;
		fullScanCompleted(_time);
	}
	
	void MeduzzzaCommonWidget::stoppedSlot() 
	{
		m_started = false;
		m_paused = false;
		stopped();
	}
	
	void MeduzzzaCommonWidget::pausedSlot() 
	{
		m_paused = true;
		paused();
	}
	
	void MeduzzzaCommonWidget::resumedSlot() 
	{
		m_paused = false;
		resumed();
	}
	
}
