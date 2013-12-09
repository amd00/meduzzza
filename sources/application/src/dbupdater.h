
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza.
 *
 *  meduzzza is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DBUPDATER_H_
#define _DBUPDATER_H_

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QDir>
#include <QMap>
#include <QDateTime>

namespace Meduzzza
{
	class Manager;
	
	class DbUpdater : public QObject
	{
		Q_OBJECT
		
	private:
		struct DownloadItem
		{
			DownloadItem() {}
			DownloadItem(QFile*_file, const QDateTime &_time) :
				file(_file), start_time(_time) {}
			QFile *file;
			QDateTime start_time;
		};
		
	private:
		Manager *m_man;
		QString m_mirror;
		QString m_db_path;
		QDir m_db_tmp_dir;
		QMap<QNetworkReply*, DownloadItem> m_pool;
		QNetworkAccessManager m_nam;
		QNetworkProxy m_proxy;
		QDateTime m_start_time;
		bool m_is_full;

	public:
		DbUpdater(Manager *_man, const QString &_mirror, const QString &_db_path, QNetworkProxy::ProxyType _proxy_type, const QString &_proxy_host, qint16 _proxy_port,
					const QString &_proxy_user, const QString &_proxy_password);
		~DbUpdater() {}
		void update();
		void dailyUpdate();
		
	private:
		void downloadFiles(const QStringList &_files);
		
	public Q_SLOTS:
		void dbUpdateMirrorChangedSlot(const QString &_val);
		void proxyTypeChangedSlot(QNetworkProxy::ProxyType _val);
		void proxyHostChangedSlot(const QString &_val);
		void proxyPortChangedSlot(qint16 _val);
		void proxyUserChangedSlot(const QString &_val);
		void proxyPasswordChangedSlot(const QString &_val);

	private Q_SLOTS:
		void downloadFinishedSlot();
		void readyReadSlot();
		void updateProgressSlot(qint64 _read, qint64 _total);
		void errorSlot(QNetworkReply::NetworkError _error);
		
// 	Q_SIGNALS:
// 		void downloadStartedSignal(const QString &_file);
// 		void downloadFinishedSignal(const QString &_file);
// 		void downloadProgressSignal(const QString &_file, qint64 _read, qint64 _total);
// 		void updateCompletedSignal();
// 		void errorSignal(const QString &_file_name, const QString &_error_string);
	};
}
#endif
