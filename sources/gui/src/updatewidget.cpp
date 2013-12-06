
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
		m_ui -> m_start_button -> setDisabled(true);
	}
	
	void UpdateWidget::updateCompleted(const QDateTime &_start_time, const QDateTime &_end_time)
	{
		m_ui -> m_start_button -> setDisabled(false);
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

	void UpdateWidget::startUpdateSlot()
	{
		if(m_ui -> m_full_check -> checkState() == Qt::Checked)
			m_man -> downloadDb();
		else
			m_man -> updateDb();
	}

}