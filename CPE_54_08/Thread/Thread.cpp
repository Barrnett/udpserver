#include "Thread.h"

CThread::CThread(START_ROUTINE func)
{
	m_bExist = false;
	m_bIsStart  = false;
	m_func	    = func;
	m_bInit = false;
	m_bExit = true;
}

CThread::~CThread()
{
	stop();
}

bool CThread::start()
{
	if(m_bExist) return false;

	m_bExit = false;
	pthread_t tid;
	pthread_attr_t attr;
	int err;
	do
	{
		err = pthread_attr_init(&attr);
		if(err != 0) break;
		err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		if(err != 0) break;
		err = pthread_create(&tid, NULL, m_func, this);
		if(err != 0) break;
		m_bExist = true;
	}
	while(false);
	pthread_attr_destroy(&attr);
	if(err != 0)
	{
		m_bExit = true; 
		return false;
	}

	while(!m_bIsStart) 
	{
		usleep(10000);
	}
	return true;
}

bool CThread::stop()
{
	m_bExist = false;
	m_bExit = true; 
	m_bIsStart = false;

	return true;
}

bool CThread::isStart()
{
	return m_bIsStart;
}

bool CThread::isQuit()
{
	if (m_bExit)
		return true;

	return false;
}
