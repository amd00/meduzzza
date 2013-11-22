
#include <manager.h>

#include <ui_memscanwidget.h>

#include "mainwindow.h"
#include "memscanwidget.h"
#include <QDebug>
namespace Meduzzza
{

	MemScanWidget::MemScanWidget(Manager *_man, MainWindow *_med) : 
			MeduzzzaCommonWidget(_man, _med), m_ui(new Ui::MemScanWidget), 
			m_started(false), m_paused(false)
	{
		m_ui -> setupUi(this);
	}

	MemScanWidget::~MemScanWidget() { delete m_ui; }

	void MemScanWidget::startClickedSlot()
	{
		qDebug() << "START";
		if(!m_started)
			m_man -> scanMemory();
		else if(!m_paused)
			m_man -> pause();
		else
			m_man -> resume();
	}
	
	void MemScanWidget::stopClickedSlot()
	{
		m_man -> stop();
	}
	
	void MemScanWidget::procScanStartedSlot(const QString &_name, qint32 _pid)
	{
		m_ui -> m_proc_label -> setText(QString("%1(%2)").arg(_name).arg(_pid));
	}

	void MemScanWidget::procScanCompletedSlot(const QString &_name, qint32 _pid)
	{
	}

	void MemScanWidget::procVirusDetectedSlot(const QString &_name, qint32 _pid, const QString &_virus)
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
