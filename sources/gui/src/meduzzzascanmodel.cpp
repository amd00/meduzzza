
#include <manager.h>

#include "meduzzzascanmodel.h"

namespace Meduzzza
{
	
	MeduzzzaScanModel *MeduzzzaScanModel::ms_self = NULL;
		
	MeduzzzaScanModel *MeduzzzaScanModel::get()
	{
		if(ms_self == NULL)
			ms_self = new MeduzzzaScanModel;
		return ms_self;
	}

	MeduzzzaScanModel::MeduzzzaScanModel() : QAbstractItemModel()
	{
		Manager *_man = Manager::get();
		connect(_man, SIGNAL(fileScanStartedSignal(const QString&)), this, SLOT(fileScanStartedSlot(const QString&)));
		connect(_man, SIGNAL(fileScanCompletedSignal(const QString&)), this, SLOT(fileScanCompletedSlot(const QString&)));
		connect(_man, SIGNAL(fileVirusDetectedSignal(const QString&, const QString&)), 
				this, SLOT(fileVirusDetectedSlot(const QString&, const QString&)));
		
		connect(_man, SIGNAL(procScanStartedSignal(const QString&, Q_PID)), this, SLOT(procScanStartedSlot(const QString&, Q_PID)));
		connect(_man, SIGNAL(procScanCompletedSignal(const QString&, Q_PID)), this, SLOT(procScanCompletedSlot(const QString&, Q_PID)));
		connect(_man, SIGNAL(procVirusDetectedSignal(const QString&, Q_PID, const QString&)), 
				this, SLOT(procVirusDetectedSlot(const QString&, Q_PID, const QString&)));
		
		connect(_man, SIGNAL(dirScanStartedSignal(const QString&)), this, SLOT(dirScanStartedSlot(const QString&)));
		connect(_man, SIGNAL(dirScanCompletedSignal(const QString&)), this, SLOT(dirScanCompletedSlot(const QString&)));
		
		connect(_man, SIGNAL(memScanStartedSignal()), this, SLOT(memScanStartedSlot()));
		connect(_man, SIGNAL(memScanCompletedSignal()), this, SLOT(memScanCompletedSlot()));
		
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
					res = item.name;
					break;
				case Pid:
					res = item.pid == 0 ? QVariant() : item.pid;
					break;
				case Status:
					res = item.virname.isNull() ? tr("Scanning...") : (item.virname.isEmpty() ? tr("Clean") : tr("Infected"));
					break;
				case VirName:
					res = item.virname;
					break;
				}
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
				case VirName:
					res = tr("Virus");
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
	
		void MeduzzzaScanModel::fileScanStartedSlot(const QString &_file)
		{
			beginInsertRows(QModelIndex(), rowCount(), rowCount());
			ScanItem item(_file);
			m_items << item;
			endInsertRows();
		}
		
		void MeduzzzaScanModel::fileScanCompletedSlot(const QString &_file) 
		{
			qint32 i = findItem(_file);
			if(i == -1)
				return;
			m_items[i].virname = "";
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::VirName, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::fileVirusDetectedSlot(const QString &_file, const QString &_virname) 
		{
			qint32 i = findItem(_file);
			if(i == -1)
				return;
			m_items[i].virname = _virname;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::VirName, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::procScanStartedSlot(const QString &_name, Q_PID _pid) 
		{
			beginInsertRows(QModelIndex(), rowCount(), rowCount());
			ScanItem item(_name, _pid);
			m_items << item;
			endInsertRows();
		}
		
		void MeduzzzaScanModel::procScanCompletedSlot(const QString &_name, Q_PID _pid) 
		{
			qint32 i = findItem(_name, _pid);
			if(i == -1)
				return;
			m_items[i].virname = "";
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::VirName, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QString &_virname) 
		{
			qint32 i = findItem(_name, _pid);
			if(i == -1)
				return;
			m_items[i].virname = _virname;
			QModelIndex ind_begin = index(i, MeduzzzaScanModel::Status, QModelIndex());
			QModelIndex ind_end = index(i, MeduzzzaScanModel::VirName, QModelIndex());
			Q_EMIT dataChanged(ind_begin, ind_end);
		}
		
		void MeduzzzaScanModel::stoppedSlot() 
		{
			
		}
		
}


