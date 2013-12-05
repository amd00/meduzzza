
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza-gui.
 *
 *  meduzzza-gui is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza-gui is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza-gui.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QPainter>

#include <manager.h>

#include <ui_updatewidget.h>

#include "updatewidget.h"
#include "updatemodel.h"
namespace Meduzzza
{
	
	void ProgressDelegate::paint(QPainter *_painter, const QStyleOptionViewItem &_option, const QModelIndex &_index) const
	{
		if(!_index.isValid() || _index.column() != UpdateModel::Progress)
		{
			QItemDelegate::paint(_painter, _option, _index);
			return;
		}
		QStyleOptionProgressBarV2 opts;
		opts.palette = _option.palette;
		opts.rect = _option.rect;
		opts.minimum = 0;
		opts.maximum = 100;
		opts.progress = _index.data().toInt();
	
		QApplication::style() -> drawControl(QStyle::CE_ProgressBar, &opts, _painter, 0);
	}

	UpdateWidget:: UpdateWidget(MainWindow *_med) : MeduzzzaCommonWidget(_med),
		m_ui(new Ui::UpdateWidget), m_model(new UpdateModel), m_delegate(new ProgressDelegate)
	{
		m_ui -> setupUi(this);
		m_ui -> m_age_label -> setText(QString::number(m_man -> virusDbAge()));
		m_ui -> m_update_view -> setModel(m_model);
		m_ui -> m_update_view -> setItemDelegate(m_delegate);
	}

	UpdateWidget::~UpdateWidget() 
	{ 
		delete m_ui; 
		delete m_model;
	}

	void UpdateWidget::updateStarted(bool _is_full, const QDateTime &_start_time)
	{
	}
	
	void UpdateWidget::updateCompleted(const QDateTime &_start_time, const QDateTime &_end_time)
	{
	}
	
	void UpdateWidget::fileDownloadStarted(const QString &_file, const QDateTime &_start_time)
	{
	}
	
	void UpdateWidget::fileDownloadCompleted(const QString &_file, const QDateTime &_start_time, const QDateTime &_end_time)
	{
	}
	
	void UpdateWidget::fileDownloadProgress(const QString &_file, quint64 _read, quint64 _total)
	{
	}

// 	void UpdateWidget::downloadStartedSlot(const QString &_file)
// 	{
// 		QLabel *label(new QLabel(_file));
// 		QProgressBar *bar(new QProgressBar());
// 		bar -> setMinimum(0);
// 		bar -> setMaximum(100);
// 		m_items[_file] = qMakePair(label, bar);
// 		qint32 row = m_ui -> m_items_layout -> rowCount();
// 		m_ui -> m_items_layout -> addWidget(label, row, 0);
// 		m_ui -> m_items_layout -> addWidget(bar, row, 1);
// 	}
// 
// 	void UpdateWidget::downloadFinishedSlot(const QString &_file)
// 	{
// 		m_items[_file].second -> setValue(100);
// 	}
// 
// 	void UpdateWidget::downloadProgressSlot(const QString &_file, qint64 _read, qint64 _total)
// 	{
// 		m_items[_file].second -> setValue((qreal)_read / _total * 100);
// 	}
// 
	void UpdateWidget::startUpdateSlot()
	{
		m_man -> updateDb();
	}
// 
// 	void UpdateWidget::updateCompletedSlot()
// 	{
// 		m_ui -> m_age_label -> setText(QString::number(m_man -> virusDbAge()));
// 	}

}