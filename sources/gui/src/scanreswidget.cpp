
#include <manager.h>

#include "scanreswidget.h"

namespace Meduzzza
{

	ScanResWidget::ScanResWidget() : QWidget(), m_ui()
	{
		m_ui.setupUi(this);
		
// 		quint64 health = _files - _infected;
		Statist *statist = Manager::get() -> statist();
		m_ui.m_files_label -> setText("<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">" + 
				QString::number(statist -> filesCount()) + 
				" files scanned</span></p></body></html>");
		m_ui.m_healthinfo_label -> setText(QString::number(statist -> filesCount() - statist -> fileVirusesCount()));
		m_ui.m_infinfo_label -> setText(QString::number(statist -> fileVirusesCount()));
		m_ui.m_quarinfo_label -> setText(QString::number(statist -> quarantined()));
		
		qint32 health_str = (float)statist -> filesCount() - statist -> fileVirusesCount() / (float)statist -> filesCount() * 100;
		qint32 inf_str = (float)statist -> fileVirusesCount() / (float)statist -> filesCount() * 100;
		qint32 quar_str = (float)statist -> quarantined() / (float)statist -> filesCount() * 100;
		m_ui.m_diagram_layout -> setStretch(0, health_str);
		m_ui.m_diagram_layout -> setStretch(1, inf_str);
		m_ui.m_diagram_layout -> setStretch(2, quar_str);
	}

}