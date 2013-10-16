
#include <QFileDialog>

#include <ui_dirscanwidget.h>
#include <manager.h>

#include "meduzzza.h"
#include "dirscanwidget.h"

DirScanWidget::DirScanWidget(Manager *_man, Meduzzza *_med, QWidget *_parent) : 
		QWidget(_parent), m_ui(new Ui::DirScanWidget), m_man(_man), m_med(_med)
{
	m_ui -> setupUi(this);
	connect(m_man, SIGNAL(dirScanCompletedSignal()), this, SLOT(dirScanCompletedSlot()));
}

DirScanWidget::~DirScanWidget() { delete m_ui; }

void DirScanWidget::dirClickedSlot()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(!dir.isEmpty())
		m_ui -> m_dir_edit -> setText(dir);
}

void DirScanWidget::startClickedSlot()
{
	connect(m_man, SIGNAL(fileScanStartedSignal(const QString&)), 
			this, SLOT(fileScanStartedSlot(const QString&)));
	connect(m_man, SIGNAL(fileScanCompletedSignal(const QString&)), 
			this, SLOT(fileScanCompletedSlot(const QString&)));
	connect(m_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QString&)), 
			this, SLOT(fileVirusDetectedSlot(const QString&, const QString&)));
	m_man -> scanDir(m_ui -> m_dir_edit -> text());
}

void DirScanWidget::dirScanCompletedSlot()
{
	disconnect(m_man, SIGNAL(fileScanStartedSignal(const QString&)), 
			this, SLOT(fileScanStartedSlot(const QString&)));
	disconnect(m_man, SIGNAL(fileScanCompletedSignal(const QString&)), 
			this, SLOT(fileScanCompletedSlot(const QString&)));
	disconnect(m_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QString&)), 
			this, SLOT(fileVirusDetectedSlot(const QString&, const QString&)));
}

void DirScanWidget::fileScanStartedSlot(const QString &_file)
{
	m_ui -> m_file_label -> setText(_file);
}

void DirScanWidget::fileScanCompletedSlot(const QString &_file)
{
}

void DirScanWidget::fileVirusDetectedSlot(const QString &_file, const QString &_virus)
{
}
