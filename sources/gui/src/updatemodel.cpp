
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

#include <manager.h>

#include "updatemodel.h"

namespace Meduzzza
{
	UpdateModel::UpdateModel() : QAbstractItemModel(), m_items() 
	{
		Manager *man = Manager::get();
		connect(man, SIGNAL(updateStartedSignal(bool, const QDateTime&)), this, SLOT(updateStartedSlot(bool, const QDateTime&)));
		connect(man, SIGNAL(updateCompletedSignal(const QDateTime&, const QDateTime&)), 
				this, SLOT(updateCompletedSlot(const QDateTime&, const QDateTime&)));
		connect(man, SIGNAL(fileDownloadStartedSignal(const QString&, const QDateTime&)),
				this, SLOT(fileDownloadStartedSlot(const QString&, const QDateTime&)));
		connect(man, SIGNAL(fileDownloadCompletedSignal(const QString&, const QDateTime&, const QDateTime&)),
				this, SLOT(fileDownloadCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
		connect(man, SIGNAL(fileDownloadProgressSignal(const QString&, quint64, quint64)),
				this, SLOT(fileDownloadProgressSlot(const QString&, quint64, quint64)));
		connect(man, SIGNAL(fileDownloadErrorSignal(const QString&, const QString&)),
				this, SLOT(fileDownloadErrorSlot(const QString&, const QString&)));
	}
	
	QVariant UpdateModel::data(const QModelIndex &_ind, int _role) const
	{
		QVariant res;
		if(!_ind.isValid())
			return res;
		DownloadItem item = m_items[_ind.row()];
		switch(_role)
		{
		case Qt::DisplayRole:
			switch(_ind.column())
			{
			case Name:
				res = _ind.data(Qt::UserRole + Name);
				break;
			case Status:
				switch(_ind.data(Qt::UserRole + Status).toInt())
				{
				case InProgress:
					res = tr("Downloading...");
					break;
				case Downloaded:
					res = tr("Downloaded");
					break;
				case Error:
					res = tr("Error");
					break;
				}
				break;
			case Progress:
				res = _ind.data(Qt::UserRole + Progress);
				break;
			case StartTime:
				res = QLocale().toString(_ind.data(Qt::UserRole + StartTime).toDateTime(), QLocale::ShortFormat);
				break;
			case EndTime:
				res = QLocale().toString(_ind.data(Qt::UserRole + EndTime).toDateTime(), QLocale::ShortFormat);
				break;
			}
			break;
		case Qt::UserRole + Name:
			res = item.name;
			break;
		case Qt::UserRole + Status:
			res = item.status;
			break;
		case Qt::UserRole + Progress:
			res = item.total == 0 ? 0 : (qreal)item.read / (qreal)item.total * 100;
			break;
		case Qt::UserRole + StartTime:
			res = item.start_time;
			break;
		case Qt::UserRole + EndTime:
			res = item.end_time;
			break;
		}
		return res;
	}
	
	QModelIndex UpdateModel::index(qint32 _row, qint32 _column, const QModelIndex &_par) const
	{
		if(_par.isValid() || _row < 0 || _row >= rowCount(_par) || 
			_column < 0 || _column >= columnCount(_par))
			return QModelIndex();
		return createIndex(_row, _column, (void*)&m_items.at(_row));
	}
	
	QVariant UpdateModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
	{
		QVariant res;
		if(_orientation != Qt::Horizontal)
			return res;
		switch(_role)
		{
		case Qt::DisplayRole:
			switch(_section)
			{
			case Name:
				res = tr("Name");
				break;
			case Status:
				res = tr("Status");
				break;
			case Progress:
				res = tr("Progress");
				break;
			case StartTime:
				res = tr("Start time");
				break;
			case EndTime:
				res = tr("End time");
				break;
			}
			break;
		default:
			break;
		}
		return res;
	}
	
	void UpdateModel::clear()
	{
		beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
		m_items.clear();
		endRemoveRows();
	}
	
	qint32 UpdateModel::findItem(const QString &_file)
	{
		for(qint32 i = 0; i < m_items.size(); i++)
			if(m_items[i].name == _file)
				return i;
		return -1;
	}
	
	void UpdateModel::updateStartedSlot(bool _is_full, const QDateTime &_start_time)
	{
		clear();
	}
	
	void UpdateModel::updateCompletedSlot(const QDateTime &_start_time, const QDateTime &_end_time)
	{
	}
	
	void UpdateModel::fileDownloadStartedSlot(const QString &_file, const QDateTime &_start_time)
	{
		DownloadItem item(_file);
		item.start_time = _start_time;
		beginInsertRows(QModelIndex(), rowCount(), rowCount());
		m_items << item;
		endInsertRows();
	}
	
	void UpdateModel::fileDownloadCompletedSlot(const QString &_file, const QDateTime &_start_time, const QDateTime &_end_time)
	{
		qint32 i = findItem(_file);
		if(i == -1)
			return;
		m_items[i].status = UpdateModel::Downloaded;
		m_items[i].end_time = _end_time;
		QModelIndex ind_begin = index(i, UpdateModel::Status, QModelIndex());
		QModelIndex ind_end = index(i, UpdateModel::EndTime, QModelIndex());
		Q_EMIT dataChanged(ind_begin, ind_end);
	}
	
	void UpdateModel::fileDownloadProgressSlot(const QString &_file, quint64 _read, quint64 _total)
	{
		qint32 i = findItem(_file);
		if(i == -1)
			return;
		m_items[i].read = _read;
		m_items[i].total = _total;
		QModelIndex ind_begin = index(i, UpdateModel::Progress, QModelIndex());
		Q_EMIT dataChanged(ind_begin, ind_begin);
	}
	
	void UpdateModel::fileDownloadErrorSlot(const QString &_file, const QString &_error)
	{
		qint32 i = findItem(_file);
		if(i == -1)
			return;
		m_items[i].status = UpdateModel::Error;
		QModelIndex ind_begin = index(i, UpdateModel::Status, QModelIndex());
		Q_EMIT dataChanged(ind_begin, ind_begin);
	}
}