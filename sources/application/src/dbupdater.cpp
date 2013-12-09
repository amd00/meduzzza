
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

#include <QStringList>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCoreApplication>

#include "dbupdater.h"
#include "manager.h"
#include "meduzzzaevent.h"

namespace Meduzzza
{

	DbUpdater::DbUpdater(Manager *_man, const QString &_mirror, const QString &_db_path, QNetworkProxy::ProxyType _proxy_type, 
						const QString &_proxy_host, qint16 _proxy_port, const QString &_proxy_user, const QString &_proxy_password) : 
						QObject(), m_man(_man), m_mirror(_mirror), m_db_path(_db_path), m_db_tmp_dir(), m_pool(), m_nam(), 
						m_proxy(_proxy_type, _proxy_host, _proxy_port, _proxy_user, _proxy_password)
	{
		m_nam.setProxy(m_proxy);
		m_db_tmp_dir.setPath(QDir(m_db_path).absoluteFilePath("db_update"));
		m_db_tmp_dir.mkpath(m_db_tmp_dir.absolutePath());
	}

	void DbUpdater::update()
	{
		downloadFiles(QStringList() << "main.cvd" << "daily.cvd" << "bytecode.cvd" << "safebrowsing.cvd");
	}

	void DbUpdater::dailyUpdate()
	{
		downloadFiles(QStringList() << "daily.cvd");
	}

	void DbUpdater::downloadFiles(const QStringList &_files)
	{
		m_start_time = QDateTime::currentDateTime();
		UpdateStartedEvent *start_event(new UpdateStartedEvent(_files.size() > 1, m_start_time));
		QCoreApplication::postEvent(m_man, start_event);
		foreach(QString file, _files)
		{
			QUrl url(m_mirror + "/" + file);
			QFile *f = new QFile(m_db_tmp_dir.absoluteFilePath(file));
			f -> open(QIODevice::WriteOnly | QIODevice::Truncate);
			QNetworkReply *reply = m_nam.get(QNetworkRequest(url));
			m_pool[reply] = DownloadItem(f, QDateTime::currentDateTime());
			connect(reply, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));
			connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
			connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgressSlot(qint64, qint64)));
			connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorSlot(QNetworkReply::NetworkError)));
			FileDownloadStartedEvent *file_event(new FileDownloadStartedEvent(file, QDateTime::currentDateTime()));
			QCoreApplication::postEvent(m_man, file_event);
		}
	}

	void DbUpdater::dbUpdateMirrorChangedSlot(const QString &_val)
	{
		m_mirror = _val;
	}

	void DbUpdater::proxyTypeChangedSlot(QNetworkProxy::ProxyType _val)
	{
		m_proxy.setType(_val);
		m_nam.setProxy(m_proxy);
	}

	void DbUpdater::proxyHostChangedSlot(const QString &_val)
	{
		m_proxy.setHostName(_val);
		m_nam.setProxy(m_proxy);
	}

	void DbUpdater::proxyPortChangedSlot(qint16 _val)
	{
		m_proxy.setPort(_val);
		m_nam.setProxy(m_proxy);
	}

	void DbUpdater::proxyUserChangedSlot(const QString &_val)
	{
		m_proxy.setUser(_val);
		m_nam.setProxy(m_proxy);
	}

	void DbUpdater::proxyPasswordChangedSlot(const QString &_val)
	{
		m_proxy.setPassword(_val);
		m_nam.setProxy(m_proxy);
	}

	void DbUpdater::downloadFinishedSlot()
	{
		QNetworkReply *reply = (QNetworkReply*)sender();
		QFile *f = m_pool[reply].file;
		f -> close();
		QString file_name = QFileInfo(*f).fileName();
		QDir db_dir(m_db_path);
		QFile::remove(db_dir.absoluteFilePath(file_name));
		f -> rename(db_dir.absoluteFilePath(file_name));
		
		FileDownloadCompletedEvent *file_event(new FileDownloadCompletedEvent(file_name, m_pool[reply].start_time, QDateTime::currentDateTime()));
		QCoreApplication::postEvent(m_man, file_event);
		
		m_pool.remove(reply);
		reply -> deleteLater();
		delete f;
		
		if(m_pool.isEmpty())
		{
			UpdateCompletedEvent *end_event(new UpdateCompletedEvent(m_start_time, QDateTime::currentDateTime()));
			QCoreApplication::postEvent(m_man, end_event);
		}
	}

	void DbUpdater::readyReadSlot()
	{
		QNetworkReply *reply = (QNetworkReply*)sender();
		m_pool[reply].file -> write(reply -> readAll());
	}

	void DbUpdater::updateProgressSlot(qint64 _read, qint64 _total)
	{
		QNetworkReply *reply = (QNetworkReply*)sender();
		QFile *f = m_pool[reply].file;
		QString file_name = QFileInfo(*f).fileName();
		FileDownloadProgressEvent *file_event(new FileDownloadProgressEvent(file_name, _read, _total));
		QCoreApplication::postEvent(m_man, file_event);
	}

	void DbUpdater::errorSlot(QNetworkReply::NetworkError _error)
	{
		QNetworkReply *reply = (QNetworkReply*)sender();
		QFile *f = m_pool[reply].file;
		QString file_name = QFileInfo(*f).fileName();
		switch(_error)
		{
			case QNetworkReply::NoError:
				break;
			default:
				disconnect(reply, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));
				disconnect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
				disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgressSlot(qint64, qint64)));
				f -> remove();
				m_pool.remove(reply);
				reply -> deleteLater();
				FileDownloadErrorEvent *error_event(new FileDownloadErrorEvent(file_name, reply -> errorString()));
				QCoreApplication::postEvent(m_man, error_event);
				if(m_pool.isEmpty())
				{
					UpdateCompletedEvent *end_event(new UpdateCompletedEvent(m_start_time, QDateTime::currentDateTime()));
					QCoreApplication::postEvent(m_man, end_event);
				}
				break;
		};
	}

}