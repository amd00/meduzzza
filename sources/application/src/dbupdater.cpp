
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

#include "dbupdater.h"

DbUpdater::DbUpdater(const QString &_mirror, const QString &_db_path, bool _proxy, const QString &_proxy_host, qint16 _proxy_port,
				const QString &_proxy_user, const QString &_proxy_password) : QObject(), m_mirror(_mirror), m_db_path(_db_path), 
				m_db_tmp_dir(), m_pool(), m_man(), 
				m_proxy(_proxy ? QNetworkProxy::HttpProxy : QNetworkProxy::NoProxy, _proxy_host, _proxy_port, _proxy_user, _proxy_password)
{
	m_man.setProxy(m_proxy);
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
	foreach(QString file, _files)
	{
		QUrl url;
		url.setScheme("http");
		url.setHost(m_mirror);
		url.setPath("/" + file);
		QFile *f = new QFile(m_db_tmp_dir.absoluteFilePath(file));
		f -> open(QIODevice::WriteOnly | QIODevice::Truncate);
		QNetworkReply *reply = m_man.get(QNetworkRequest(url));
		m_pool[reply] = f;
		connect(reply, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));
		connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
		connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgressSlot(qint64,qint64)));
		connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorSlot(QNetworkReply::NetworkError)));
		Q_EMIT downloadStartedSignal(file);
	}
}

void DbUpdater::dbUpdateMirrorChangedSlot(const QString &_val)
{
	m_mirror = _val;
}

void DbUpdater::hasProxyChangedSlot(bool _val)
{
	m_proxy.setType(_val ? QNetworkProxy::HttpProxy : QNetworkProxy::NoProxy);
	m_man.setProxy(m_proxy);
}

void DbUpdater::proxyHostChangedSlot(const QString &_val)
{
	m_proxy.setHostName(_val);
	m_man.setProxy(m_proxy);
}

void DbUpdater::proxyPortChangedSlot(qint16 _val)
{
	m_proxy.setPort(_val);
	m_man.setProxy(m_proxy);
}

void DbUpdater::proxyUserChangedSlot(const QString &_val)
{
	m_proxy.setUser(_val);
	m_man.setProxy(m_proxy);
}

void DbUpdater::proxyPasswordChangedSlot(const QString &_val)
{
	m_proxy.setPassword(_val);
	m_man.setProxy(m_proxy);
}

void DbUpdater::downloadFinishedSlot()
{
	QNetworkReply *reply = (QNetworkReply*)sender();
	QFile *f = m_pool[reply];
	f -> close();
	QString file_name = QFileInfo(*f).fileName();
	QDir db_dir(m_db_path);
	QFile::remove(db_dir.absoluteFilePath(file_name));
	f -> rename(db_dir.absoluteFilePath(file_name));
	m_pool.remove(reply);
	reply -> deleteLater();
	delete f;
	Q_EMIT downloadFinishedSignal(file_name);
	if(m_pool.isEmpty())
		Q_EMIT updateCompletedSignal();
}

void DbUpdater::readyReadSlot()
{
	QNetworkReply *reply = (QNetworkReply*)sender();
	m_pool[reply] -> write(reply -> readAll());
}

void DbUpdater::updateProgressSlot(qint64 _read, qint64 _total)
{
	QNetworkReply *reply = (QNetworkReply*)sender();
	QFile *f = m_pool[reply];
	QString file_name = QFileInfo(*f).fileName();
// 	qDebug("Download %s: %i : %i", file_name.toLocal8Bit().data(), _read, _total);
	Q_EMIT downloadProgressSignal(file_name, _read, _total);
}

void DbUpdater::errorSlot(QNetworkReply::NetworkError _error)
{
	QNetworkReply *reply = (QNetworkReply*)sender();
	QFile *f = m_pool[reply];
	QString file_name = QFileInfo(*f).fileName();
	switch(_error)
	{
		case QNetworkReply::NoError:
			break;
		default:
			disconnect(reply, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));
			disconnect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
			disconnect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgressSlot(qint64,qint64)));
			QDir db_dir(m_db_path);
			QFile::remove(db_dir.absoluteFilePath(file_name));
			f -> rename(db_dir.absoluteFilePath(file_name));
			m_pool.remove(reply);
			reply -> deleteLater();
			Q_EMIT errorSignal(file_name, reply -> errorString());
			break;
	};
}
