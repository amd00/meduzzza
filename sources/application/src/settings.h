
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

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <QSettings>
#include <QDir>
#include <QNetworkProxy>

namespace Meduzzza
{
	class Settings : public QSettings
	{
		Q_OBJECT

	public:
		Settings(const QDir &_dir) : QSettings(_dir.absoluteFilePath("meduzzza.conf"), QSettings::IniFormat) {}
		~Settings() {}
		
		bool autoQuarantine() const { return value("quarantine/auto_quarantine", false).toBool(); }
		void setAutoQuarantine(bool _val) { setValue("quarantine/auto_quarantine", _val);  Q_EMIT autoQuarantineChangedSignal(_val); }
		
		qint32 maxDbAge() const { return value("database/max_age", 7).toInt(); }
		void setMaxDbAge(qint32 _val) { setValue("database/max_age", _val); Q_EMIT maxDbAgeChangedSignal(_val); }
		
		QString dbUpdateMirror() { return value("database/mirror", "http://database.clamav.net").toString(); }
		void setDbUpdateMirror(const QString _val) { setValue("database/mirror", _val); Q_EMIT dbUpdateMirrorChangedSignal(_val); }
		
		QNetworkProxy::ProxyType proxyType() const { return (QNetworkProxy::ProxyType)value("database/proxy", QNetworkProxy::NoProxy).toInt(); }
		void setProxyType(QNetworkProxy::ProxyType _val) { setValue("database/proxy", _val); Q_EMIT proxyTypeChangedSignal(_val); }
		
		QString proxyHost() const { return value("database/proxy_host", "127.0.0.1").toString(); }
		void setProxyHost(const QString &_val) { setValue("database/proxy_host", _val); Q_EMIT proxyHostChangedSignal(_val); }
		
		qint16 proxyPort() const { return value("database/proxy_port", 3128).toInt(); }
		void setProxyPort(qint16 _val) { setValue("database/proxy_port", _val); Q_EMIT proxyPortChangedSignal(_val); }
		
		QString proxyUser() const { return value("database/proxy_user", QString()).toString(); }
		void setProxyUser(const QString &_val) { setValue("database/proxy_user", _val); Q_EMIT proxyUserChangedSignal(_val); }
		
		QString proxyPassword() const { return value("database/proxy_password", QString()).toString(); }
		void setProxyPassword(const QString &_val) { setValue("database/proxy_password", _val); Q_EMIT proxyPasswordChangedSignal(_val); }
		
		qint32 threadCount() const { return value("scanner/thread_count", -1).toInt(); }
		void setThreadCount(qint32 _val) { setValue("scanner/thread_count", _val); Q_EMIT threadCountChanged(_val); }
		
	Q_SIGNALS:
		void autoQuarantineChangedSignal(bool _val);
		void maxDbAgeChangedSignal(qint32 _val);
		void dbUpdateMirrorChangedSignal(const QString &_val);
		void proxyTypeChangedSignal(QNetworkProxy::ProxyType _val);
		void proxyHostChangedSignal(const QString &_val);
		void proxyPortChangedSignal(qint16 _val);
		void proxyUserChangedSignal(const QString &_val);
		void proxyPasswordChangedSignal(const QString &_val);
		void threadCountChanged(qint32 _val);
		
	};
}
#endif
