#include "Work/DbSend.h"
#include "Work/DbRead.h"
#include "Work/fileRead.h"
#include "Work/TaskManager.h"
#include "Tool/YCodec.h"

#include "MxHandlePool.h"
#include "MXDatabase/NetElementDb.h"

CDbRead dbRead;
Task taskDispatch;
NetConfig netConfig;


#include <iostream>
using namespace std;

#define _IP_MARK "."

int main(int argc, char* argv[])
{
	//读取配置文件
	ReadConfig configInfo("./config/config-54");
	if( !configInfo.getContent() )
	{
		perror("config load failed!");
		return -1;
	}
	
	netConfig.loadConfig(configInfo);
	//初始化读取线程
	dbRead.Initialize();

	//初始化任务调度线程
	taskDispatch.Initialize();

	while (1)
	{
		usleep(1000000);
	}
 

	//测试
	/*
	string strError;
	list<NETELEMENTLINE> lstLines;
	if(CDb::getLines(lstLines,strError))
	{
		printf("get lines success\n");
	}
	else
	{
		printf("get lines failed:%s\n",strError.c_str());
	}
	*/


	/*
	string strError;
	std::list<CPU> listRecords;
	if(CDb::getTableData(listRecords,strError)){
		printf("get records success\n");
	}else{
		printf("get records failed:%s\n",strError.c_str());
	}
	*/



	/*
	CDb cdb;
	string SN;
	string strError;
	if(cdb.getControllerID(SN, strError))
	{
		printf("get records success\n");
	}
	else
	{
		printf("get records failed:%s\n",strError.c_str());
	}
	*/

	return 0;
}

