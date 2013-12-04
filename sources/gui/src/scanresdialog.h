
#ifndef _SCANRESDIALOG_H_
#define _SCANRESDIALOG_H_

#include <QDialog>

namespace Ui
{
	class ScanResDialog;
}

namespace Meduzzza
{
	class ScanResDialog : public QDialog
	{
		Q_OBJECT
		
	private:
		Ui::ScanResDialog *m_ui;
		
	public:
		ScanResDialog(bool _is_mem);
		~ScanResDialog();
	};
}
#endif