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

	void setTask(CDbSend * sendTask);//�������
	void startTask();//ִ������
	int getTaskState(CDbSend * pTask);//��ȡ����״̬��0δ��ʼ��1���ڽ��У�2�����
	void clearTask();//�������
private:
	TaskList m_listTask;
};

#endif
