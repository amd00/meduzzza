
#include "scanreswidget.h"

namespace Meduzzza
{

	ScanResWidget::ScanResWidget(quint64 _files, quint64 _infected, quint64 _quarantined, QWidget *_par) : QWidget(), m_ui()
	{
		m_ui.setupUi(this);
		
		quint64 health = _files - _infected;
		
		m_ui.m_files_label -> setText("<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">" + QString::number(_files) + " files scanned</span></p></body></html>");
		m_ui.m_healthinfo_label -> setText(QString::number(health));
		m_ui.m_infinfo_label -> setText(QString::number(_infected));
		m_ui.m_quarinfo_label -> setText(QString::number(_quarantined));
		
		qint32 health_str = (float)health / (float)_files * 100;
		qint32 inf_str = (float)_infected / (float)_files * 100;
		qint32 quar_str = (float)_quarantined / (float)_files * 100;
		m_ui.m_diagram_layout -> setStretch(0, health_str);
		m_ui.m_diagram_layout -> setStretch(1, inf_str);
		m_ui.m_diagram_layout -> setStretch(2, quar_str);
	}

}