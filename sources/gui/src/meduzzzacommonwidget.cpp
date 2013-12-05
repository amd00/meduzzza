
#include <manager.h>

#include "meduzzzacommonwidget.h"
#include "mainwindow.h"

namespace Meduzzza
{

	MeduzzzaCommonWidget::MeduzzzaCommonWidget(MainWindow *_mw) : QWidget(), m_man(Manager::get()), m_mw(_mw),
			m_started(false), m_paused(false)
	{
			connect(m_man, SIGNAL(fileScanStartedSignal(const QString&, const QDateTime&)), 
					this, SLOT(fileScanStartedSlot(const QString&, const QDateTime&)));
			connect(m_man, SIGNAL(fileScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
					this, SLOT(fileScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
			connect(m_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)), 
					this, SLOT(fileVirusDetectedSlot(const QString&, const QDateTime&, const QDateTime&, const QString&)));
			
			connect(m_man, SIGNAL(procScanStartedSignal(const QString&, Q_PID, const QDateTime&)), 
					this, SLOT(procScanStartedSlot(const QString&, Q_PID, const QDateTime&)));
			connect(m_man, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&)), 
					this, SLOT(procScanCompletedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&)));
			connect(m_man, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)), 
					this, SLOT(procVirusDetectedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)));
			connect(m_man, SIGNAL(procScanErrorSignal(const QString&, Q_PID, const QString&)), 
					this, SLOT(procScanErrorSlot(const QString&, Q_PID, const QString&)));
			
			connect(m_man, SIGNAL(dirScanStartedSignal(const QString&, const QDateTime&)), 
					this, SLOT(dirScanStartedSlot(const QString&, const QDateTime&)));
			connect(m_man, SIGNAL(dirScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
					this, SLOT(dirScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
			
			connect(m_man, SIGNAL(memScanStartedSignal(const QDateTime&)), this, SLOT(memScanStartedSlot(const QDateTime&)));
			connect(m_man, SIGNAL(memScanCompletedSignal(const QDateTime&, const QDateTime&)), 
					this, SLOT(memScanCompletedSlot(const QDateTime&, const QDateTime&)));
			
			connect(m_man, SIGNAL(fullScanStartedSignal(const QDateTime&)), this, SLOT(fullScanStartedSlot(const QDateTime&)));
			connect(m_man, SIGNAL(fullScanCompletedSignal(const QDateTime&, const QDateTime&)), 
					this, SLOT(fullScanCompletedSlot(const QDateTime&, const QDateTime&)));
			
			connect(m_man, SIGNAL(stoppedSignal()), this, SLOT(stoppedSlot()));
			connect(m_man, SIGNAL(pausedSignal()), this, SLOT(pausedSlot()));
			connect(m_man, SIGNAL(resumedSignal()), this, SLOT(resumedSlot()));
			
			connect(m_man, SIGNAL(filesFoundSignal(quint64)), this, SLOT(filesFoundSlot(quint64)));
			connect(m_man, SIGNAL(procsFoundSignal(quint64)), this, SLOT(procsFoundSlot(quint64)));
			
			connect(m_man, SIGNAL(updateStartedSignal(bool, const QDateTime&)), this, SLOT(updateStartedSlot(bool, const QDateTime&)));
			connect(m_man, SIGNAL(updateCompletedSignal(const QDateTime&, const QDateTime&)), 
					this, SLOT(updateCompletedSlot(const QDateTime&, const QDateTime&)));
			connect(m_man, SIGNAL(fileDownloadStartedSignal(const QString&, const QDateTime&)),
					this, SLOT(fileDownloadStartedSlot(const QString&, const QDateTime&)));
			connect(m_man, SIGNAL(fileDownloadCompletedSignal(const QString&, const QDateTime&, const QDateTime&)),
					this, SLOT(fileDownloadCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
			connect(m_man, SIGNAL(fileDownloadProgressSignal(const QString&, quint64, quint64)),
					this, SLOT(fileDownloadProgressSlot(const QString&, quint64, quint64)));
	}
	
	void MeduzzzaCommonWidget::dirScanStartedSlot(const QString &_dir, const QDateTime &_time_start) 
	{
		m_started = true;
		m_paused = false;
		dirScanStarted(_dir);
	}
	
	void MeduzzzaCommonWidget::dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end) 
	{
		m_started = false;
		m_paused = false;
		dirScanCompleted(_dir);
	}
	
	void MeduzzzaCommonWidget::memScanStartedSlot(const QDateTime &_time_start) 
	{
		m_started = true;
		m_paused = false;
		memScanStarted();
	}
	
	void MeduzzzaCommonWidget::memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end) 
	{
		m_started = false;
		m_paused = false;
		memScanCompleted();
	}
	
	void MeduzzzaCommonWidget::fullScanStartedSlot(const QDateTime &_time_start) 
	{
		m_started = true;
		m_paused = false;
		fullScanStarted(_time_start);
	}
	
	void MeduzzzaCommonWidget::fullScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end) 
	{
		m_started = false;
		m_paused = false;
		fullScanCompleted(_time_end);
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
