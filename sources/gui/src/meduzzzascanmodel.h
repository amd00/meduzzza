
#ifndef _MEDUZZZASCANMODEL_H_
#define _MEDUZZZASCANMODEL_H_

#include <QAbstractItemModel>
#include <QProcess>

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
			Stopped
		};
		
		enum Columns
		{
			Name = 0,
			Pid,
			Status,
			VirName
		};
		
	private:
		struct ScanItem
		{
			ScanItem(QString _file) : name(_file), pid(0), 
				virname(QString::null), status(MeduzzzaScanModel::Scanning) {}
			ScanItem(QString _name, Q_PID _pid) : name(_name), pid(_pid), 
				virname(QString::null), status(MeduzzzaScanModel::Scanning) {}
			QString name;
			Q_PID pid;
			QString virname;
			qint32 status;
		};
		
	private:
		QList<ScanItem> m_items;
		static MeduzzzaScanModel *ms_self;
		
	public:
		static MeduzzzaScanModel *get();
		
		virtual ~MeduzzzaScanModel() {}
	
		virtual QVariant data(const QModelIndex &_ind, int _role) const;
		virtual qint32 rowCount(const QModelIndex &_par = QModelIndex()) const { return m_items.size(); }
		virtual qint32 columnCount(const QModelIndex &_ind) const { return 4; }
		virtual QModelIndex index(qint32 _row, qint32 _column, const QModelIndex &_par) const;
		virtual QModelIndex parent(const QModelIndex &_ind) const { return QModelIndex(); }
		virtual QVariant headerData(int section, Qt::Orientation orientation, int _role) const;
		
		void clear();
		
	private:
		MeduzzzaScanModel();
		qint32 findItem(const QString &_file);
		qint32 findItem(const QString &_name, Q_PID _pid);
	
	protected Q_SLOTS:
		virtual void fileScanStartedSlot(const QString &_file);
		virtual void fileScanCompletedSlot(const QString &_file);
		virtual void fileVirusDetectedSlot(const QString &_file, const QString &_virname);
		
		virtual void procScanStartedSlot(const QString &_name, Q_PID _pid);
		virtual void procScanCompletedSlot(const QString &_name, Q_PID _pid);
		virtual void procVirusDetectedSlot(const QString &_name, Q_PID _pid, const QString &_virname);
		
		virtual void dirScanStartedSlot(const QString &_dir) {}
		virtual void dirScanCompletedSlot(const QString &_dir) {}
		
		virtual void memScanStartedSlot() {}
		virtual void memScanCompletedSlot() {}
		
		virtual void stoppedSlot();
		virtual void pausedSlot() {}
		virtual void resumedSlot() {}
		
	};
}

#endif
