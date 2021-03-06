
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
			FileDownloadProgress,
			FileDownloadError
		};
		
	public:
		MeduzzzaEvent(MeduzzzaEventType _type) : QEvent((QEvent::Type)_type) {}
		virtual ~MeduzzzaEvent() {}
	};
	
	class UpdateStartedEvent : public MeduzzzaEvent
	{
	private:
		bool m_is_full;
		QDateTime m_start_time;
		
	public: 
		UpdateStartedEvent(bool _is_full, const QDateTime &_start_time) : 
			MeduzzzaEvent(MeduzzzaEvent::UpdateStarted), m_is_full(_is_full), m_start_time(_start_time) {}
		~UpdateStartedEvent() {}
		
		bool isFull() const { return m_is_full; }
		QDateTime startTime() const { return m_start_time; }
	};
	
	class UpdateCompletedEvent : public MeduzzzaEvent
	{
	private:
		QDateTime m_start_time;
		QDateTime m_end_time;
		
	public:
		UpdateCompletedEvent(const QDateTime &_start_time, const QDateTime &_end_time) : 
				MeduzzzaEvent(MeduzzzaEvent::UpdateCompleted), m_start_time(_start_time),
				m_end_time(_end_time) {}
		~UpdateCompletedEvent() {}
		
		QDateTime startTime() const { return m_start_time; }
		QDateTime endTime() const { return m_end_time; }
	};
	
	class FileDownloadStartedEvent : public MeduzzzaEvent
	{
	private:
		QString m_file;
		QDateTime m_start_time;
		
	public:
		FileDownloadStartedEvent(const QString &_file, const QDateTime &_start_time) :
				MeduzzzaEvent(MeduzzzaEvent::FileDownloadStarted),
				m_file(_file), m_start_time(_start_time) {}
		~FileDownloadStartedEvent() {}
		
		QString file() const { return m_file; }
		QDateTime startTime() const { return m_start_time; }
	};
	
	class FileDownloadCompletedEvent : public MeduzzzaEvent
	{
	private:
		QString m_file;
		QDateTime m_start_time;
		QDateTime m_end_time;
		
	public:
		FileDownloadCompletedEvent(const QString &_file, const QDateTime &_start_time, const QDateTime &_end_time) :
				MeduzzzaEvent(MeduzzzaEvent::FileDownloadCompleted),
				m_file(_file), m_start_time(_start_time), m_end_time(_end_time) {}
		~FileDownloadCompletedEvent() {}
		
		QString file() const { return m_file; }
		QDateTime startTime() const { return m_start_time; }
		QDateTime endTime() const { return m_end_time; }
	};
	
	class FileDownloadProgressEvent : public MeduzzzaEvent
	{
	private:
		QString m_file;
		quint64 m_read;
		quint64 m_total;
		
	public:
		FileDownloadProgressEvent(const QString &_file, quint64 _read, quint64 _total) :
				MeduzzzaEvent(MeduzzzaEvent::FileDownloadProgress), m_file(_file),
				m_read(_read), m_total(_total) {}
		~FileDownloadProgressEvent() {}
		
		QString file() const { return m_file; }
		quint64 read() const { return m_read; }
		quint64 total() const { return m_total; }
	};
	
	class FileDownloadErrorEvent : public MeduzzzaEvent
	{
	private:
		QString m_file;
		QString m_error;
		
	public:
		FileDownloadErrorEvent(const QString &_file, const QString &_error) :
			MeduzzzaEvent(MeduzzzaEvent::FileDownloadError), m_file(_file), m_error(_error) {}
		~FileDownloadErrorEvent() {}
		
		QString file() const { return m_file; }
		QString error() const { return m_error; }
	};
}
#endif
