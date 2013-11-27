
#include <QMutex>
#include <QWaitCondition>

#include "scanner.h"

namespace Meduzzza
{
	class ScannerPrivate
	{
	private:
		static bool m_is_stopped;
		static bool m_is_paused;
		static QMutex m_mutex;
		static QWaitCondition m_pause_manager;

	public:
		ScannerPrivate() {}
		~ScannerPrivate() {}
		
		static bool stopped() { return ScannerPrivate::m_is_stopped; }
		static bool paused() { return ScannerPrivate::m_is_paused; }
		static QMutex &mutex() { return ScannerPrivate::m_mutex; }
		static QWaitCondition &pauseManager() { return ScannerPrivate::m_pause_manager; }
		
		static void setStopped(bool _is_stopped) { ScannerPrivate::m_is_stopped = _is_stopped; }
		static void setPaused(bool _is_paused) { ScannerPrivate::m_is_paused = _is_paused; }
	};
	
	bool ScannerPrivate::m_is_stopped(false);
	bool ScannerPrivate::m_is_paused(false);
	QMutex ScannerPrivate::m_mutex;
	QWaitCondition ScannerPrivate::m_pause_manager;
	
//============================================================================================//	
	
	Scanner::Scanner() : m_p(new ScannerPrivate) {}
	
	Scanner::~Scanner() { delete m_p; }
	
	void Scanner::run()
	{
		QThread::currentThread() -> setPriority(QThread::LowestPriority);
		runThread();
	}
	
	void Scanner::stop() { ScannerPrivate::setStopped(true); if(ScannerPrivate::paused()) Scanner::resume(); }
	
	void Scanner::pause() { ScannerPrivate::setPaused(true); }
	
	void Scanner::resume()
	{
		ScannerPrivate::setPaused(false);
		ScannerPrivate::mutex().lock();
		ScannerPrivate::pauseManager().wakeAll();
		ScannerPrivate::mutex().unlock();
	}
	
	bool Scanner::stopped() { return ScannerPrivate::stopped(); }
	
	void Scanner::setStopped(bool _is_stopped) { ScannerPrivate::setStopped(_is_stopped); }
		
	void Scanner::checkPause()
	{ 
		while(ScannerPrivate::paused())
		{
			ScannerPrivate::mutex().lock();
			ScannerPrivate::pauseManager().wait(&ScannerPrivate::mutex()); 
			ScannerPrivate::mutex().unlock();
		}
	}
}
