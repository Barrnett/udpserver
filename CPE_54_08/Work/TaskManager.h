#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <list>
#include "DbSend.h"
#include "Container.h"
using namespace std;

typedef std::list<CDbSend*> TaskList;

#ifdef WIN32
#define	THREAD_TYPE DWORD WINAPI
#else
#define	THREAD_TYPE void *
#endif

extern "C"
{
	THREAD_TYPE TaskThread(LPVOID lpPara);
}

class Task : public CThread  
{
	friend THREAD_TYPE TaskThread(LPVOID lpPara);
public:
	Task();
	virtual ~Task();

	bool Initialize();
	void UnInitialize();

	void setTask(CDbSend * sendTask);//添加任务
	void startTask();//执行任务
	int getTaskState(CDbSend * pTask);//获取任务状态，0未开始，1正在进行，2已完成
	void clearTask();//清除任务
private:
	TaskList m_listTask;
};

#endif
