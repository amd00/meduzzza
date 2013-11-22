
#include <manager.h>

#include "meduzzzacommonwidget.h"
#include "mainwindow.h"

namespace Meduzzza
{

	MeduzzzaCommonWidget::MeduzzzaCommonWidget(Manager *_man, MainWindow *_mw) : QWidget(), m_man(_man), m_mw(_mw)
	{
			connect(m_man, SIGNAL(fileScanStartedSignal(const QString&)), this, SLOT(fileScanStartedSlot(const QString&)));
			connect(m_man, SIGNAL(fileScanCompletedSignal(const QString&)), this, SLOT(fileScanCompletedSlot(const QString&)));
			connect(m_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QString&)), 
					this, SLOT(fileVirusDetectedSlot(const QString&, const QString&)));
			
			connect(m_man, SIGNAL(procScanStartedSignal(const QString&, qint32)), this, SLOT(procScanStartedSlot(const QString&, qint32)));
			connect(m_man, SIGNAL(procScanCompletedSignal(const QString&, qint32)), this, SLOT(procScanCompletedSlot(const QString&, qint32)));
			connect(m_man, SIGNAL(procVirusDetectedSignal(const QString&, qint32, const QString&)), 
					this, SLOT(procVirusDetectedSlot(const QString&, qint32, const QString&)));
			
			connect(m_man, SIGNAL(dirScanStartedSignal(const QString&)), this, SLOT(dirScanStartedSlot(const QString&)));
			connect(m_man, SIGNAL(dirScanCompletedSignal(const QString&)), this, SLOT(dirScanCompletedSlot(const QString&)));
			
			connect(m_man, SIGNAL(memScanStartedSignal()), this, SLOT(memScanStartedSlot()));
			connect(m_man, SIGNAL(memScanCompletedSignal()), this, SLOT(memScanCompletedSlot()));
			
			connect(m_man, SIGNAL(stoppedSignal()), this, SLOT(stoppedSlot()));
			connect(m_man, SIGNAL(pausedSignal()), this, SLOT(pausedSlot()));
			connect(m_man, SIGNAL(resumedSignal()), this, SLOT(resumedSlot()));
	}
	
}
