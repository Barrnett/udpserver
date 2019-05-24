#ifndef LOCAL_FAULT_H
#define LOCAL_FAULT_H

#include <string>
using namespace std;
#include "../MXDatabase/NetElementDb.h"

class CLocalFault
{
public:
	CLocalFault();
	~CLocalFault();
	void Clear();

	bool CheckCommand(char* data);
	bool Response(int flowNumber);
	void  charIntToByte(int i, char *bytes);
	
	bool LocalFauNums(LOCALFAULT localfauinfo);
	bool FaultRecoId(LOCALFAULT localfauinfo);
	bool FaultTypeCode(LOCALFAULT localfauinfo);
	bool FaultTime(LOCALFAULT localfauinfo);
	bool FaultRecoTime(LOCALFAULT localfauinfo);
	bool WarnTypeLen(LOCALFAULT localfauinfo);
	bool WarnTypeName(LOCALFAULT localfauinfo);
	bool WarnDescLen(LOCALFAULT localfauinfo);
	bool WarnDescri(LOCALFAULT localfauinfo);
	
private:
	char m_HEAD[6+1];   //帧头标志 HEAD
	char m_DA[1+1];     //信宿标识 DA
	char m_SA[1+1];     //信源标识 SA
	char m_LEN[2+1];    //帧长度 LEN
	char m_FT[1+1];     //帧命令类型 FT
	char m_SEQ[4+1];    //帧流水号 SEQ
	char m_FCS[2+1];    //帧校验 FCS

	//
	char m_ErroeReportNums[1+1];    //本次故障告警记录数量M
    char m_ErrorReportId[4+1];      //故障告警记录ID
	char m_ErrorType[4+1];          //故障告警类型编码
	char m_OccurTime[5+1];          //发生时间
	char m_RecoverTime[5+1];        //恢复时间
	char m_ErrorTypeNameLen[1+1];   //告警类型名称长度
	char m_ErrorTypeName[32+1];     //告警类型名称
	char m_ErrorDesLen[1+1];        //告警描述长度
	char m_ErrorDes[32+1];          //告警描述

public:
	char m_szResponsePackage[256];
	int  m_nResponsePackageLen;

};
#endif  //LOCAL_FAULT_H