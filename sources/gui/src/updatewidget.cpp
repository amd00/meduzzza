
#include <QProgressBar>
#include <QLabel>

#include <manager.h>

#include <ui_updatewidget.h>

#include "updatewidget.h"

namespace Meduzzza
{

	UpdateWidget:: UpdateWidget(MainWindow *_med, QWidget *_parent) : QWidget(_parent),
		m_ui(new Ui::UpdateWidget), m_items(), m_man(Manager::get()), m_med(_med)
	{
		m_ui -> setupUi(this);
		connect(m_man, SIGNAL(downloadStartedSignal(const QString&)), this, SLOT(downloadStartedSlot(const QString&)));
		connect(m_man, SIGNAL(downloadFinishedSignal(const QString&)), this, SLOT(downloadFinishedSlot(const QString&)));
		connect(m_man, SIGNAL(downloadProgressSignal(const QString&, qint64, qint64)), 
				this, SLOT(downloadProgressSlot(const QString&, qint64, qint64)));
		connect(m_man, SIGNAL(updateCompletedSignal()), this, SLOT(updateCompletedSlot()));
		m_ui -> m_age_label -> setText(QString::number(m_man -> virusDbAge()));
	}

	UpdateWidget::~UpdateWidget() { delete m_ui; }

	void UpdateWidget::downloadStartedSlot(const QString &_file)
	{
		QLabel *label(new QLabel(_file));
		QProgressBar *bar(new QProgressBar());
		bar -> setMinimum(0);
		bar -> setMaximum(100);
		m_items[_file] = qMakePair(label, bar);
		qint32 row = m_ui -> m_items_layout -> rowCount();
		m_ui -> m_items_layout -> addWidget(label, row, 0);
		m_ui -> m_items_layout -> addWidget(bar, row, 1);
	}

	void UpdateWidget::downloadFinishedSlot(const QString &_file)
	{
		m_items[_file].second -> setValue(100);
	}

	void UpdateWidget::downloadProgressSlot(const QString &_file, qint64 _read, qint64 _total)
	{
		m_items[_file].second -> setValue((qreal)_read / _total * 100);
	}

	void UpdateWidget::startUpdateSlot()
	{
		m_items.clear();
		m_man -> downloadDb();
	}

	void UpdateWidget::updateCompletedSlot()
	{
		m_ui -> m_age_label -> setText(QString::number(m_man -> virusDbAge()));
	}

}