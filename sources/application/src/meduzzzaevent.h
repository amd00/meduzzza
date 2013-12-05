
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of clamavqtengine.
 *
 *  clamavqtengine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  clamavqtengine is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with clamavqtengine.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MEDUZZZAEVENT_H_
#define _MEDUZZZAEVENT_H_

#include <QEvent>
#include <QDateTime>

namespace Meduzzza
{
	class MeduzzzaEvent : public QEvent
	{
	public:
		enum MeduzzzaEventType
		{
			UpdateStarted = QEvent::User + 23456,
			UpdateCompleted,
			UpdateError,
			FileDownloadStarted,
			FileDownloadCompleted,
			FileDownloadProgress
		};
		
	public:
		MeduzzzaEvent(MeduzzzaEventType _type) : QEvent((QEvent::Type)_type) {}
		virtual ~MeduzzzaEvent() {}
	};
	
	class UpdateStartedEvent : public MeduzzzaEvent
	{
	private:
		bool m_is_full;
		QDateTime m_time;
		
	public: 
		UpdateStartedEvent(bool _is_full, const QDateTime &_time) : 
			MeduzzzaEvent(MeduzzzaEvent::UpdateStarted), m_is_full(_is_full), m_time(_time) {}
		~UpdateStartedEvent() {}
		
		bool isFull() const { return m_is_full; }
		QDateTime time() const { return m_time; }
	};
	
	class UpdateCompletedEvent : public MeduzzzaEvent
	{
	private:
		QDateTime m_time;
		
	public:
		UpdateCompletedEvent(const QDateTime &_time) : 
				MeduzzzaEvent(MeduzzzaEvent::UpdateCompleted), m_time(_time) {}
		~UpdateCompletedEvent() {}
		
		QDateTime time() const { return m_time; }
	};
	
	class FileDownloadStartedEvent : public MeduzzzaEvent
	{
	private:
		QString m_name;
		QDateTime m_time;
		
	public:
		FileDownloadStartedEvent(const QString &_name, const QDateTime &_time) :
				MeduzzzaEvent(MeduzzzaEvent::FileDownloadStarted),
				m_name(_name), m_time(_time) {}
		~FileDownloadStartedEvent() {}
		
		QString name() const { return m_name; }
		QDateTime time() const { return m_time; }
	};
	
	class FileDownloadCompletedEvent : public MeduzzzaEvent
	{
	private:
		QString m_name;
		QDateTime m_time;
		
	public:
		FileDownloadCompletedEvent(const QString &_name, const QDateTime &_time) :
				MeduzzzaEvent(MeduzzzaEvent::FileDownloadCompleted),
				m_name(_name), m_time(_time) {}
		~FileDownloadCompletedEvent() {}
		
		QString name() const { return m_name; }
		QDateTime time() const { return m_time; }
	};
	
	class FileDownloadProgressEvent : public MeduzzzaEvent
	{
	private:
		QString m_name;
		quint64 m_read;
		quint64 m_total;
		
	public:
		FileDownloadProgressEvent(const QString &_name, quint64 _read, quint64 _total) :
				MeduzzzaEvent(MeduzzzaEvent::FileDownloadProgress), m_name(_name),
				m_read(_read), m_total(_total) {}
		~FileDownloadProgressEvent() {}
		
		QString name() const { return m_name; }
		quint64 read() const { return m_read; }
		quint64 total() const { return m_total; }
	};
}
#endif
