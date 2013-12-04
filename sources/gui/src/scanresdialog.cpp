
#include <manager.h>

#include <ui_scanresdialog.h>

#include "scanresdialog.h"

namespace Meduzzza
{

	ScanResDialog::ScanResDialog(bool _is_mem) : QDialog(), m_ui(new Ui::ScanResDialog)
	{
		m_ui -> setupUi(this);
		Statist *statist = Manager::get() -> statist();
		if(_is_mem)
		{
			m_ui -> m_files_label -> setText("<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">" + 
					QString::number(statist -> procsCount()) + 
					" processes scanned</span></p></body></html>");
			m_ui -> m_healthinfo_label -> setText(QString::number(statist -> procsCount() - statist -> procVirusesCount()));
			m_ui -> m_infinfo_label -> setText(QString::number(statist -> procVirusesCount()));
			m_ui -> m_quarinfo_label -> hide();
			m_ui -> m_quar_label -> hide();
			m_ui -> m_otherinfo_label -> setText(QString::number(statist -> procErrorsCount()));
			
			qint32 health_str = ((float)statist -> procsCount() - statist -> procVirusesCount()) / (float)statist -> procsCount() * 100;
			qint32 inf_str = (float)statist -> procVirusesCount() / (float)statist -> procsCount() * 100;
			qint32 other_str = (float)statist -> procErrorsCount() / (float)statist -> procsCount() * 100;
			m_ui -> m_diagram_layout -> setStretch(0, health_str);
			m_ui -> m_diagram_layout -> setStretch(1, inf_str);
			m_ui -> m_diagram_layout -> setStretch(2, other_str);
		}
		else
		{
			m_ui -> m_files_label -> setText("<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">" + 
					QString::number(statist -> filesCount()) + 
					" files scanned</span></p></body></html>");
			m_ui -> m_healthinfo_label -> setText(QString::number(statist -> filesCount() - statist -> fileVirusesCount()));
			m_ui -> m_infinfo_label -> setText(QString::number(statist -> fileVirusesCount()));
			m_ui -> m_quarinfo_label -> setText(QString::number(statist -> quarantined()));
			m_ui -> m_other_label -> hide();
			m_ui -> m_otherinfo_label -> hide();
			
			qint32 health_str = ((float)statist -> filesCount() - statist -> fileVirusesCount()) / (float)statist -> filesCount() * 100;
			qint32 inf_str = (float)statist -> fileVirusesCount() / (float)statist -> filesCount() * 100;
			qint32 quar_str = (float)statist -> quarantined() / (float)statist -> filesCount() * 100;
			m_ui -> m_diagram_layout -> setStretch(0, health_str);
			m_ui -> m_diagram_layout -> setStretch(1, inf_str);
			m_ui -> m_diagram_layout -> setStretch(2, quar_str);
		}
	}
	
	ScanResDialog::~ScanResDialog() { delete m_ui; }

}
