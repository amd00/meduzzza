
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
#include <settings.h>

#include <ui_setupwidget.h>
#include <ui_meduzzzacommonwidget.h>

#include "setupwidget.h"

namespace Meduzzza
{
	SetupWidget::SetupWidget(MainWindow *_med) : MeduzzzaCommonWidget(_med), m_ui(new Ui::SetupWidget),
			m_settings(man() -> settings())
	{
		m_ui -> setupUi(m_base_ui -> m_widget);
		m_ui -> m_proxytype_combo -> addItem(tr("No proxy"), QNetworkProxy::NoProxy);
		m_ui -> m_proxytype_combo -> addItem(tr("Socks5"), QNetworkProxy::Socks5Proxy);
		m_ui -> m_proxytype_combo -> addItem(tr("HTTP"), QNetworkProxy::HttpCachingProxy);
		m_ui -> m_proxytype_combo -> addItem(tr("Ftp"), QNetworkProxy::FtpCachingProxy);
		
		connect(m_ui -> m_autoquar_check, SIGNAL(stateChanged(qint32)), this, SLOT(autoquarStateChanged(qint32)));
		connect(m_ui -> m_proxytype_combo, SIGNAL(currentIndexChanged(qint32)), this, SLOT(proxytypeCurrentIndexChanged(qint32)));
		connect(m_ui -> m_proxysrv_edit, SIGNAL(textEdited(const QString&)), 
				this, SLOT(proxysrvTextEdited(const QString&)));
		connect(m_ui -> m_proxyport_edit, SIGNAL(textEdited(const QString&)), 
				this, SLOT(proxyportTextEdited(const QString&)));
		connect(m_ui -> m_proxyuser_edit, SIGNAL(textEdited(const QString&)), 
				this, SLOT(proxyuserTextEdited(const QString&)));
		connect(m_ui -> m_proxypassword_edit, SIGNAL(textEdited(const QString&)), 
				this, SLOT(proxypasswordTextEdited(const QString&)));
		connect(m_ui -> m_dbsrv_edit, SIGNAL(textEdited(const QString&)), 
				this, SLOT(dbsrvTextEdited(const QString&)));
		
		m_ui -> m_autoquar_check -> setCheckState(m_settings -> autoQuarantine() ? Qt::Checked : Qt::Unchecked);
		qint32 ind = m_ui -> m_proxytype_combo -> findData(m_settings -> proxyType());
		m_ui -> m_proxytype_combo -> setCurrentIndex(ind);
		proxytypeCurrentIndexChanged(ind);
		m_ui -> m_proxysrv_edit -> setText(m_settings -> proxyHost());
		m_ui -> m_proxyport_edit -> setText(QString::number(m_settings -> proxyPort()));
		m_ui -> m_proxyuser_edit -> setText(m_settings -> proxyUser());
		m_ui -> m_proxypassword_edit -> setText(m_settings -> proxyPassword());
		m_ui -> m_dbsrv_edit -> setText(m_settings -> dbUpdateMirror());
	}
	
	SetupWidget::~SetupWidget() { delete m_ui; }
	
	void SetupWidget::autoquarStateChanged(qint32 _state)
	{
		m_settings -> setAutoQuarantine(m_ui -> m_autoquar_check -> checkState() == Qt::Checked);
	}
	
	void SetupWidget::proxytypeCurrentIndexChanged(qint32 _index)
	{
		QNetworkProxy::ProxyType type = (QNetworkProxy::ProxyType)m_ui -> m_proxytype_combo -> itemData(_index).toInt();
		if(type == QNetworkProxy::NoProxy)
		{
			m_ui -> m_proxysrv_edit -> setDisabled(true);
			m_ui -> m_proxyport_edit -> setDisabled(true);
			m_ui -> m_proxyuser_edit -> setDisabled(true);
			m_ui -> m_proxypassword_edit -> setDisabled(true);
		}
		else
		{
			m_ui -> m_proxysrv_edit -> setDisabled(false);
			m_ui -> m_proxyport_edit -> setDisabled(false);
			m_ui -> m_proxyuser_edit -> setDisabled(false);
			m_ui -> m_proxypassword_edit -> setDisabled(false);
		}
		m_settings -> setProxyType(type);
	}
	
	void SetupWidget::proxysrvTextEdited(const QString &_text)
	{
		m_settings -> setProxyHost(m_ui -> m_proxysrv_edit -> text());
	}
	
	void SetupWidget::proxyportTextEdited(const QString &_text)
	{
		m_settings -> setProxyPort(m_ui -> m_proxyport_edit -> text().toInt());
	}
	
	void SetupWidget::proxyuserTextEdited(const QString &_text)
	{
		m_settings -> setProxyUser(m_ui -> m_proxyuser_edit -> text());
	}
	
	void SetupWidget::proxypasswordTextEdited(const QString &_text)
	{
		m_settings -> setProxyPassword(m_ui -> m_proxypassword_edit -> text());
	}
	
	void SetupWidget::dbsrvTextEdited(const QString &_text)
	{
		m_settings -> setDbUpdateMirror(m_ui -> m_dbsrv_edit -> text());
	}
}