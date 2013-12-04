
#ifndef _MEDUZZZASCANMODEL_H_
#define _MEDUZZZASCANMODEL_H_

#include <QAbstractItemModel>
#include <QProcess>
#include <QDateTime>

namespace Meduzzza
{
	class Manager;
	
	class MeduzzzaScanModel : public QAbstractItemModel
	{
		Q_OBJECT
		
	public:
		enum Status
		{
			Scanning,
			Clean,
			Infected,
			Stopped,
			Error
		};
		
		enum Columns
		{
			Name = 0,
			Pid,
			Status,
			Data,
			StartTime,
			EndTime
		};
		
	private:
		struct ScanItem
		{
			ScanItem(QString _file) : name(_file), pid(0), status(MeduzzzaScanModel::Scanning) {}
			ScanItem(QString _name, Q_PID _pid) : name(_name), pid(_pid), 
				status(MeduzzzaScanModel::Scanning) {}
			QString name;
			Q_PID pid;
			QString data;
			qint32 status;
			QDateTime start_time;
			QDateTime end_time;
		};
		
	private:
		QList<ScanItem> m_items;
		bool m_proc_model;
		
	public:
		MeduzzzaScanModel(bool _proc_model);
		~MeduzzzaScanModel() {}
	
		QVariant data(const QModelIndex &_ind, int _role) const;
		qint32 rowCount(const QModelIndex &_par = QModelIndex()) const { return m_items.size(); }
		qint32 columnCount(const QModelIndex &_ind) const { return 6; }
		QModelIndex index(qint32 _row, qint32 _column, const QModelIndex &_par) const;
		QModelIndex parent(const QModelIndex &_ind) const { return QModelIndex(); }
		QVariant headerData(int section, Qt::Orientation orientation, int _role) const;
		
		void clear();
		
	private:
		qint32 findItem(const QString &_file);
		qint32 findItem(const QString &_name, Q_PID _pid);
	
	protected Q_SLOTS:
		void fileScanStartedSlot(const QString &_file, const QDateTime &_time_start);
		void fileScanCompletedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end);
		void fileVirusDetectedSlot(const QString &_file, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virname);
		
		void procScanStartedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start);
		void procScanCompletedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end);
		void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QDateTime &_time_start, const QDateTime &_time_end, const QString &_virname);
		void procScanErrorSlot(const QString &_name, Q_PID _pid, const QString &_error);
		
		void dirScanStartedSlot(const QString &_dir, const QDateTime &_time_start) {}
		void dirScanCompletedSlot(const QString &_dir, const QDateTime &_time_start, const QDateTime &_time_end) {}
		
		void memScanStartedSlot(const QDateTime &_time_start) {}
		void memScanCompletedSlot(const QDateTime &_time_start, const QDateTime &_time_end) {}
		
		void stoppedSlot();
		void pausedSlot() {}
		void resumedSlot() {}
		
	};
}

#endif
