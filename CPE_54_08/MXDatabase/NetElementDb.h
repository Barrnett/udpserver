#ifndef _NETELEMENTDB_H_
#define _NETELEMENTDB_H_

#include <stdio.h>
#include <list>
#include <string>

#include "MxHandlePool.h"
using namespace std;

typedef struct point{
	string m_strInterfaceIp;
	int    m_nstatus;
	float  m_fErrorRate;
	int    m_njitter;
	float  m_favgrtt;
	float  m_fpacketloss;
}POINTTYPE;
 
typedef struct Interfaces{
	int    m_nChannelType;
	int    m_nBandwidth;
	float  m_fBandwidthUtilization;
}INTERFACES;

typedef struct Lines{
	int    m_nStatus;
	string m_strSrcId;
	int    m_nSrcIndex;
	string m_strDstId;
	int    m_nDstIndex;
	int    m_nSrcDEVId;
	int    m_nDstDEVId;
	INTERFACES m_oInterfaces;
	POINTTYPE  m_SrcPoint;
	POINTTYPE  m_DstPoint;

	int    m_nCost;
	int    m_nSelect;
	string m_strLineId;
}NETELEMENTLINE;

typedef struct devices{
	unsigned int m_nId;
	string m_strName;
	string m_strNumber;
	string m_strSoftVersion;
	string m_strBoardIp;
	string m_strManageIp;
	int    m_nIsActivity;
	int m_nState;
	string m_strLongittude;
	string m_strLatitude;
	bool bIsSelf;

	string m_strSN;
	string m_strDescribe;
	unsigned int m_nIp;
	int m_nNumber;
}DEVICE;

typedef struct Optpath{
	int m_pathNumber;          //路径数量
	int m_pathLinkNumber;      //路径链路数量
	int m_portOut;        //控制器出端口
	int m_nextGecoid;   		   //下一跳控制器ID
	unsigned int m_nextGecoport;   //下一跳控制器入端口

//	string m_strSN;
}OPTIMALPATH;

typedef struct LocFault{
	int m_faultAlarmRecord;   	 //本次故障告警记录
	int m_faultAlarmId; 	 	 //故障告警记录ID
	int m_faultAlarmCode;	     //故障告警类型编码
	string m_faultStartTime;	 //发生时间
	string m_faultRecoverTime;   //恢复时间
	int m_faultNameLen;          //告警类型名称长度
	string m_faultAlarmName;     //告警类型名称
	int m_faultAlarmLen;         //告警描述长度
	string m_faultAlarmDescri;   //告警描述

	string m_strSN;
}LOCALFAULT;

typedef struct {
	time_t tmMxTime;
	float m_fUsage;
}CPU;

class CDb
{
public:
	CDb(void);
	~CDb(void);
private:
//	CMxHandleLoc m_mxHandleLoc;
public:
	//得到搜索的设备
	static bool getSearchDevs(list<DEVICE> &lstDevs,string &strError);
	static bool getAgentIp(unsigned int &nIp,string &strIp);

	static bool getLines(std::list<NETELEMENTLINE> &lstLines,string &strError);
	
	static bool getOptimalPath(list<OPTIMALPATH> &lstOptPath,string &strError);

	static bool getLocalFault(list<LOCALFAULT> &lstFault,string &strError);
};
#endif
