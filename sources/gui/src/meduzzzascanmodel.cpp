
#include <manager.h>

#include "meduzzzascanmodel.h"

namespace Meduzzza
{
	
	MeduzzzaScanModel::MeduzzzaScanModel(bool _proc_model) : QAbstractItemModel(), m_proc_model(_proc_model)
	{
		Manager *_man = Manager::get();
		
		if(m_proc_model)
		{
			connect(_man, SIGNAL(memScanStartedSignal(const QDateTime&)), this, SLOT(memScanStartedSlot(const QDateTime&)));
			connect(_man, SIGNAL(memScanCompletedSignal(const QDateTime&, const QDateTime&)), this, SLOT(memScanCompletedSlot(const QDateTime&, const QDateTime&)));
			connect(_man, SIGNAL(procScanStartedSignal(const QString&, Q_PID, const QDateTime&)), 
				this, SLOT(procScanStartedSlot(const QString&, Q_PID, const QDateTime&)));
			connect(_man, SIGNAL(procScanCompletedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&)), 
				this, SLOT(procScanCompletedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&)));
			connect(_man, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)), 
				this, SLOT(procVirusDetectedSlot(const QString&, Q_PID, const QDateTime&, const QDateTime&, const QString&)));
			connect(_man, SIGNAL(procScanErrorSignal(const QString&, Q_PID, const QString&)), 
				this, SLOT(procScanErrorSlot(const QString&, Q_PID, const QString&)));
		}
		else
		{
			connect(_man, SIGNAL(dirScanStartedSignal(const QString&, const QDateTime&)), 
					this, SLOT(dirScanStartedSlot(const QString&, const QDateTime&)));
			connect(_man, SIGNAL(dirScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
					this, SLOT(dirScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
			connect(_man, SIGNAL(fileScanStartedSignal(const QString&, const QDateTime&)), 
					this, SLOT(fileScanStartedSlot(const QString&, const QDateTime&)));
			connect(_man, SIGNAL(fileScanCompletedSignal(const QString&, const QDateTime&, const QDateTime&)), 
					this, SLOT(fileScanCompletedSlot(const QString&, const QDateTime&, const QDateTime&)));
			connect(_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QDateTime&, const QDateTime&, const QString&)), 
					this, SLOT(fileVirusDetectedSlot(const QString&, const QDateTime&, const QDateTime&, const QString&)));
		}
		
		connect(_man, SIGNAL(stoppedSignal()), this, SLOT(stoppedSlot()));
		connect(_man, SIGNAL(pausedSignal()), this, SLOT(pausedSlot()));
		connect(_man, SIGNAL(resumedSignal()), this, SLOT(resumedSlot()));
	}
	
		QVariant MeduzzzaScanModel::data(const QModelIndex &_ind, int _role) const
		{
			QVariant res;
			if(!_ind.isValid())
				return res;
			ScanItem item = m_items[_ind.row()];
			switch(_role)
			{
			case Qt::DisplayRole:
				switch(_ind.column())
				{
				case Name:
					res = _ind.data(Qt::UserRole + Name);
					break;
				case Pid:
					res = res = _ind.data(Qt::UserRole + Pid);
					break;
				case Status:
					switch(_ind.data(Qt::UserRole + Status).toInt())
					{
					case Scanning:
						res = tr("Scanning...");
						break;
					case Clean:
						res = tr("Clean");
						break;
					case Infected:
						res = tr("Infected");
						break;
					case Stopped:
						res = tr("Stopped");
						break;
					case Error:
						res = tr("Error");
						break;
					}
					break;
				case Data:
					res = _ind.data(Qt::UserRole + Data);
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
			case Qt::UserRole + Pid:
				res = item.pid == 0 ? QVariant() : item.pid;
				break;
			case Qt::UserRole + Status:
				res = item.status;
				break;
			case Qt::UserRole + Data:
				res = item.data;
				break;
			case Qt::UserRole + StartTime:
				res = item.start_time;
				break;
			case Qt::UserRole + EndTime:
				res = item.end_time;
				break;
			default:
				break;
			}
			return res;
		}
		
		QModelIndex MeduzzzaScanModel::index(qint32 _row, qint32 _column, const QModelIndex &_par) const
		{
			if(_par.isValid() || _row < 0 || _row >= rowCount(_par) || 
				_column < 0 || _column >= columnCount(_par))
				return QModelIndex();
			return createIndex(_row, _column, (void*)&m_items.at(_row));
			
		}
		
		QVariant MeduzzzaScanModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
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
				case Pid:
					res = tr("Pid");
					break;
				case Status:
					res = tr("Status");
					break;
				case Data:
					res = tr("Data");
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
		
		void MeduzzzaScanModel::clear()
		{
			beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
			m_items.clear();
			endRemoveRows();
		}
		
		qint32 MeduzzzaScanModel::findItem(const QString &_file)
		{
			for(qint32 i = 0; i < m_items.size(); i++)
				if(m_items[i].name == _file && m_items[i].pid == 0)
					return i;
			return -1;
		}
		
		qint32 MeduzzzaScanModel::findItem(const QString &_name, Q_PID _pid) 
		{
			for(qint32 i = 0; i < m_items.size(); i++)
				if(m_items[i].name == _name && m_items[i].pid == _pid)
					return i;
			return -1;
		}
	
		void MeduzzzaScanModel::fileScanStartedSlot(const QString &_file, const QDateTime &_time_start)
		{
			ScanItem item(_file);
			item.start_time = _time_start;
			beginInsertRows(QModelIndex(), rowCount(), rowCount());
			m_items << item;
			endInsertRows();
		}
		
		void MeduzzzaScanModel::fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end) 
		{
			qint32 i = findItem(_file);
			if(i == -1)
				return;
			m_items[i].data = "";
			m_items[i].status = MeduzzzaScanModel::Clean;
			m_items[i].end_time = _time_end;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::Data, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, 
													  const QDateTime &_time_end, const QString &_virname) 
		{
			qint32 i = findItem(_file);
			if(i == -1)
				return;
			m_items[i].data = _virname;
			m_items[i].status = MeduzzzaScanModel::Infected;
			m_items[i].end_time = _time_end;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::Data, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::procScanStartedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start) 
		{
			beginInsertRows(QModelIndex(), rowCount(), rowCount());
			ScanItem item(_name, _pid);
			item.start_time = _time_start;
			m_items << item;
			endInsertRows();
		}
		
		void MeduzzzaScanModel::procScanCompletedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end) 
		{
			qint32 i = findItem(_name, _pid);
			if(i == -1)
				return;
			m_items[i].data = "";
			m_items[i].status = MeduzzzaScanModel::Clean;
			m_items[i].end_time = _time_end;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::Data, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, 
				const QDateTime &_time_end, const QString &_virname) 
		{
			qint32 i = findItem(_name, _pid);
			if(i == -1)
				return;
			m_items[i].data = _virname;
			m_items[i].status = MeduzzzaScanModel::Infected;
			m_items[i].end_time = _time_end;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::Data, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::procScanErrorSlot(const QString &_name, Q_PID _pid, const QString &_error)
		{
			qint32 i = findItem(_name, _pid);
			if(i == -1)
				return;
			m_items[i].data = _error;
			m_items[i].status = MeduzzzaScanModel::Error;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::Data, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::stoppedSlot() 
		{
			for(qint32 i = 0; i < m_items.size(); i++)
				if(m_items[i].status == MeduzzzaScanModel::Scanning)
				{
					m_items[i].status = MeduzzzaScanModel::Stopped;
					QModelIndex ind = index(i, MeduzzzaScanModel::Status, QModelIndex());
					Q_EMIT dataChanged(ind, ind);
				}
		}
		
}


