#ifndef __NEWTHREAD_H__
#define __NEWTHREAD_H__
#include <iostream>
#include <pthread.h>
using namespace std;

typedef void *(*START_ROUTINE)(void *arg);

class CThread
{
public:
	CThread(START_ROUTINE func);
	virtual ~CThread();
	
	bool start();
	bool stop();
	
	bool isStart();
	bool isQuit();

protected:
	bool m_bInit;
	bool m_bExist;
	bool m_bExit;
	bool m_bIsStart;
	START_ROUTINE m_func;
};
#endif
