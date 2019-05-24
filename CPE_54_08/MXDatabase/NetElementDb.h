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
	int m_pathNumber;          //·������
	int m_pathLinkNumber;      //·����·����
	int m_portOut;        //���������˿�
	int m_nextGecoid;   		   //��һ��������ID
	unsigned int m_nextGecoport;   //��һ����������˿�

//	string m_strSN;
}OPTIMALPATH;

typedef struct LocFault{
	int m_faultAlarmRecord;   	 //���ι��ϸ澯��¼
	int m_faultAlarmId; 	 	 //���ϸ澯��¼ID
	int m_faultAlarmCode;	     //���ϸ澯���ͱ���
	string m_faultStartTime;	 //����ʱ��
	string m_faultRecoverTime;   //�ָ�ʱ��
	int m_faultNameLen;          //�澯�������Ƴ���
	string m_faultAlarmName;     //�澯��������
	int m_faultAlarmLen;         //�澯��������
	string m_faultAlarmDescri;   //�澯����

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
	//�õ��������豸
	static bool getSearchDevs(list<DEVICE> &lstDevs,string &strError);
	static bool getAgentIp(unsigned int &nIp,string &strIp);

	static bool getLines(std::list<NETELEMENTLINE> &lstLines,string &strError);
	
	static bool getOptimalPath(list<OPTIMALPATH> &lstOptPath,string &strError);

	static bool getLocalFault(list<LOCALFAULT> &lstFault,string &strError);
};
#endif
