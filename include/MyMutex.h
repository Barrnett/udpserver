#ifndef LIBCHEN_MUTEX_H_
#define LIBCHEN_MUTEX_H_

#ifndef WIN32
	#include "Linux_Common.h"     
#endif

#include "exception.h"

#ifdef WIN32
	#ifndef _WINSOCKAPI_
	#define _WINSOCKAPI_      //���� winsock1.1
	#endif
	#include <windows.h>
#else
	#include <mutex>
	#include <semaphore.h>  
#endif 


namespace chen{

#ifdef WIN32

	class Event {
	public :
		Event()
		{
			m_event=::CreateEvent(NULL,TRUE,FALSE,"");
			if(m_event==NULL)
				throw this;
		}
		~Event()
		{
			if(m_event!=NULL)
				::CloseHandle(m_event);
		}
		bool Wait()
		{
			return (WaitForSingleObject(m_event,INFINITE)==WAIT_OBJECT_0);
		}
		bool Wait(DWORD timer)
		{
			return (WaitForSingleObject(m_event,timer)==WAIT_OBJECT_0);
		}

		BOOL Reset()
		{
			return ::ResetEvent(m_event);
		}
		BOOL Signal()
		{
			return ::SetEvent(m_event);
		}

	protected:
		HANDLE m_event;
	};

	class Mutex {
	public:
		Mutex() {
			InitializeCriticalSection(&crit);
		}
		~Mutex() {
			DeleteCriticalSection(&crit);
		}
	protected:

		void enter() {EnterCriticalSection(&crit);}
		void exit() {LeaveCriticalSection(&crit);}
		BOOL tryEnter() { return ::TryEnterCriticalSection(&crit);}
		CRITICAL_SECTION crit;
		friend class Lock;
		friend class tryLock;
		friend class dLock;
	};

	class NameMutex{
	public:
		NameMutex(){
			m_handle=::CreateMutex(NULL,FALSE,NULL);
			m_isExist=FALSE;

		}
		NameMutex(const char *Name){
			m_handle=::CreateMutex(NULL,FALSE,Name);
			m_isExist=(GetLastError()==ERROR_ALREADY_EXISTS);
		}
		~NameMutex() {
			if(m_handle!=NULL)
			{
				::CloseHandle(m_handle);
				//puts("Close NameMutex");
			}
		}

		BOOL isExist(){return m_isExist;}
		BOOL isCreateOk(){ return (m_handle!=NULL); }

	protected:

		BOOL enter() { return (::WaitForSingleObject(m_handle,INFINITE)==WAIT_OBJECT_0);}
		BOOL exit() { return ::ReleaseMutex(m_handle); }
#define CHEN_NAMEMUTEX_TYROK		WAIT_OBJECT_0
#define CHEN_NAMEMUTEX_TYRTIMEOUT		WAIT_TIMEOUT
#define CHEN_NAMEMUTEX_TYRABANDONED		WAIT_ABANDONED
#define CHEN_NAMEMUTEX_TYRFAILED		 WAIT_FAILED
		DWORD tryEnter(DWORD timer) { 
			return ::WaitForSingleObject(m_handle,timer);
		}

		BOOL m_isExist;
		HANDLE m_handle;

		friend class NameLock;
		friend class tryNameLock;


	};

#else
	class Event {   
	public:
		Event() {	
			if (pthread_mutex_init(&m_mutex, NULL) != 0  
				|| pthread_cond_init(&m_cond, NULL) != 0)
				throw this;
			m_active = false;
		};

		~Event() {	
			// if destroy mutex that initialized but never used, we got EBUSY(centOS 6.3). 
			// (why ? I guess owner is set while init, cleared when unlock, and destroy requires no owner.)
			// (and merely call unlock doesn't help, it does nothing if not locked). 
			// so we fix it so
			pthread_mutex_trylock(&m_mutex);
			pthread_mutex_unlock(&m_mutex);

			if (pthread_mutex_destroy(&m_mutex) != 0)
				perror("pthread_mutex_destroy failed in Event destructor");

			if (pthread_cond_destroy(&m_cond) != 0)
				perror("pthread_cond_destroy failed in Event destructor"); 

			m_active = false;
		};

		bool Wait() {	
			if (pthread_mutex_lock(&m_mutex) != 0)
				return false;
			bool ret = m_active || pthread_cond_wait(&m_cond, &m_mutex) == 0;
			pthread_mutex_unlock(&m_mutex);
			return ret;
		} 

		bool Wait(DWORD timer) {	
			if (timer == INFINITE)
				return Wait();

			timespec ts;
			get_future_time(ts, timer);

			if (pthread_mutex_timedlock(&m_mutex, &ts) != 0)
				return false;

			bool ret = m_active || pthread_cond_timedwait(&m_cond, &m_mutex, &ts) == 0;
			pthread_mutex_unlock(&m_mutex);
			return ret; 
		}

		bool tryWait() {	
			if (pthread_mutex_trylock(&m_mutex) != 0)
				return false;
			bool ret = m_active;
			pthread_mutex_unlock(&m_mutex);
			return ret;
		}    
		bool Reset() {
			if (pthread_mutex_lock(&m_mutex) != 0)
				return false;
			m_active = false;
			pthread_mutex_unlock(&m_mutex);
			return true;
		} 
		bool Signal() {	
			if (pthread_mutex_lock(&m_mutex) != 0)
				return false;
			m_active = true;
			pthread_cond_broadcast(&m_cond);
			pthread_mutex_unlock(&m_mutex);
			return true;
		}   
	protected:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
		bool m_active;
	};

	class AutoEvent {  
	public:
		AutoEvent() {	
			if (pthread_mutex_init(&m_mutex, NULL) != 0  
				|| pthread_cond_init(&m_cond, NULL) != 0)
				throw this;
		};

		~AutoEvent() {	
			// if destroy mutex that initialized but never used, we got EBUSY(centOS 6.3). (why ? I guess owner is set while init, cleared when unlock, and destroy requires no owner). 
			// here fix it  
			pthread_mutex_trylock(&m_mutex);
			pthread_mutex_unlock(&m_mutex);

			if (pthread_mutex_destroy(&m_mutex) != 0)
				perror("pthread_mutex_destroy failed in Event destructor");

			if (pthread_cond_destroy(&m_cond) != 0)
				perror("pthread_cond_destroy failed in Event destructor"); 
		};

		bool Wait() {	
			if (pthread_mutex_lock(&m_mutex) != 0)
				return false;
			bool ret = pthread_cond_wait(&m_cond, &m_mutex) == 0;
			pthread_mutex_unlock(&m_mutex);
			return ret;
		} 

		bool Wait(DWORD timer) {	
			if (timer == INFINITE)
				return Wait();

			timespec ts;
			get_future_time(ts, timer);

			if (pthread_mutex_timedlock(&m_mutex, &ts) != 0)
				return false;

			bool ret = pthread_cond_timedwait(&m_cond, &m_mutex, &ts) == 0;
			pthread_mutex_unlock(&m_mutex);
			return ret; 
		}

		bool Signal() {	
			if (pthread_mutex_lock(&m_mutex) != 0)
				return false;
			pthread_cond_signal(&m_cond);
			pthread_mutex_unlock(&m_mutex);
			return true;
		}   

		bool Broadcast() {
			if (pthread_mutex_lock(&m_mutex) != 0)
				return false;
			pthread_cond_broadcast(&m_cond);
			pthread_mutex_unlock(&m_mutex);
			return true;
		}

	protected:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
	};

	class Mutex : public std::recursive_mutex { 
	public:
		Mutex() {}
		~Mutex() {}
	protected:
		void enter() {	lock();	}
		void exit() {	unlock();	}
		BOOL tryEnter() { return try_lock(); }
		friend class Lock;
		friend class tryLock;
		friend class dLock;
	};

	class NameMutex {
	public:
		NameMutex() = delete;
		NameMutex(const char *name) {
			m_name = name;
			if (m_name.empty()) {
				m_name = "/noname";
			} else if (m_name[0] != '/') {
				m_name = string("/") + m_name;
			}

			m_sem = sem_open(m_name.c_str(), 0);  
			if (m_sem != NULL) {
				m_isExist = true;
				return;
			} 
			
			m_isExist = false;
			m_sem = sem_open(m_name.c_str(), O_CREAT | O_EXCL, 0644, 1);
			if (m_sem == SEM_FAILED) {
				printf("sem_open failed while constructing NameMutex named %s\n", m_name.c_str());
				perror("");
			}
		}
		~NameMutex() {
			if (m_sem) {
				sem_close(m_sem);
				if (!m_isExist)
					sem_unlink(m_name.c_str());
			}
		}

		BOOL isExist() { return m_isExist; }
		BOOL isCreateOk(){ return m_sem != NULL; }

	protected:

		BOOL enter() { return sem_wait(m_sem) == 0; }
		BOOL exit() { return sem_post(m_sem) == 0; }

		#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)    
		#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)    

		#define CHEN_NAMEMUTEX_TYROK		WAIT_OBJECT_0
		#define CHEN_NAMEMUTEX_TYRTIMEOUT		WAIT_TIMEOUT
		#define CHEN_NAMEMUTEX_TYRABANDONED		WAIT_ABANDONED
		#define CHEN_NAMEMUTEX_TYRFAILED		 WAIT_FAILED
		DWORD tryEnter(DWORD timer) { 
			if (timer == 0)
				return sem_trywait(m_sem) == 0 ? CHEN_NAMEMUTEX_TYROK : CHEN_NAMEMUTEX_TYRFAILED;
			if (timer == INFINITE)
				return enter();

			struct timespec ts;
			if (clock_gettime(CLOCK_REALTIME, &ts)== -1)
				return false;
			ts.tv_sec += timer / 1000;
			ts.tv_nsec += (timer % 1000) * 1000 * 1000;
			if (ts.tv_nsec >= 1000 * 1000 * 1000) {
				ts.tv_sec++;
				ts.tv_nsec -= 1000 * 1000 * 1000; 
			}

			return sem_timedwait(m_sem, &ts) == 0 ? CHEN_NAMEMUTEX_TYROK : CHEN_NAMEMUTEX_TYRFAILED; 
		}

		sem_t* m_sem;
		bool m_isExist;
		string m_name;

		friend class NameLock;
		friend class tryNameLock;

	};

#endif

	class Lock {
	public:
		Lock(Mutex& m) : mutex(m) {m.enter();}
		~Lock() {mutex.exit();}
	protected:
		Mutex& mutex;
	};
	
	class tryLock {
	public:
		tryLock(Mutex& m) : mutex(m) {
			isEn=m.tryEnter();
		}
		~tryLock() { if(isEn) mutex.exit();}
		BOOL tryAgain(){
			if(!isEn)
				isEn=mutex.tryEnter(); 
			return isEn; 
		}
		BOOL isEnter(){ return isEn; }
	protected:
		BOOL isEn;
		Mutex& mutex;
	};

	class dLock {
	public:
		dLock(Mutex& m,BOOL isTry) : m_mutex(m),m_isTry(isTry) {
			if(isTry)
				m_isEn=m.tryEnter();
			else
				m.enter();
		}
		~dLock() { 
			if(m_isTry)
			{
				if (m_isEn)
					m_mutex.exit();
			}else
				m_mutex.exit();
		}
		BOOL tryAgain(){
			if(m_isTry)
			{
				if(!m_isEn)
					m_isEn=m_mutex.tryEnter();
				return m_isEn;
			}
			return FALSE;
		}
		BOOL isEnter() {
			if(m_isTry)
				return m_isEn; 
			else
				return FALSE;
		}
	protected:
		BOOL m_isEn;
		BOOL m_isTry;
		Mutex & m_mutex;
	};


	class NameLock
	{
	public:
		NameLock(NameMutex &m) : m_mutex(m) { 
			if(!m.enter()) throw exception("Name lock enter failed"); 
		}
		~NameLock() { m_mutex.exit(); }

	protected:
		NameMutex& m_mutex;
	};

	class tryNameLock {
	public:
		tryNameLock(NameMutex& m,unsigned int timer) : m_mutex(m) {
			m_state=m.tryEnter(timer);
		}
		~tryNameLock() { if(m_state==CHEN_NAMEMUTEX_TYROK) m_mutex.exit();}
		BOOL tryAgain(unsigned int timer){
			if(m_state!=CHEN_NAMEMUTEX_TYROK)
				m_state=m_mutex.tryEnter(timer); 
			return (m_state==CHEN_NAMEMUTEX_TYROK); 
		}
		BOOL isEnter(){ return (m_state==CHEN_NAMEMUTEX_TYROK); }
	protected:
		unsigned int m_state;
		NameMutex& m_mutex;
	};


}


#endif // !defined(AFX_MYLOCK_H__292AF879_1C60_4130_9902_3A064D3724A5__INCLUDED_)
