#include "TaskManager.h"
#include "DbSend.h"

THREAD_TYPE TaskThread(LPVOID lpPara)
{
	Task *pThread = (Task*)lpPara;
	if (pThread == NULL)
	{
		return (void *)-1;
	}

	pThread->m_bIsStart = true;

	while(!pThread->m_bExit)
	{
		pThread->startTask();
		pThread->clearTask();

		Sleep(1000);
	}
	cout << "TaskThread stop"<<endl;
	return (void *)0;
}

Task::Task():CThread(TaskThread)
{
}

Task::~Task()
{
}

bool Task::Initialize()
{
	m_bInit = true;

	if(!start())
	{
		UnInitialize();
		return false;
	}

	return true;
}

void Task::UnInitialize()
{
	if(!m_bInit) 
		return;
	if(isStart())
	{
		//停止线程
		stop();

	}
}

void Task::setTask(CDbSend *sendTask)
{
	m_listTask.push_back(sendTask);
	return;
}
/*
*  说明：遍历任务队列，找出新的任务执行，正在进行和已经完成的任务直接跳过。
*/
void Task::startTask()
{
	for(TaskList::iterator iter=m_listTask.begin();iter!=m_listTask.end();iter++)
	{
		CDbSend *sendTask = *iter;
		if( getTaskState(sendTask)==0 )
		{
			sendTask->Initialize();
		}
	}
	return;
}


int Task::getTaskState(CDbSend * task)
{
	return task->m_nState;	
}
/*
*	说明：清除已完成的任务。
*/
void Task::clearTask()
{
	for(TaskList::iterator iter=m_listTask.begin();iter!=m_listTask.end();)
	{
		CDbSend *sendTask = *iter;
		if( getTaskState(sendTask)==2 )
		{	
			delete sendTask;
			m_listTask.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	return;
}
