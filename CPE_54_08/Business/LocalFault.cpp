#include "LocalFault.h"
#include <assert.h>
#include <memory.h>
#include <string.h>
#include "../Tool/YCodec.h"

CLocalFault::CLocalFault()
{
	Clear();
}

CLocalFault::~CLocalFault()
{
	Clear();
}

void CLocalFault::Clear()
{
	m_nResponsePackageLen = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));

	memset(m_HEAD, 0, sizeof(m_HEAD));
	memset(m_DA, 0, sizeof(m_DA));
	memset(m_SA, 0, sizeof(m_SA));
	memset(m_LEN, 0, sizeof(m_LEN));
	memset(m_FT, 0, sizeof(m_FT));
	memset(m_SEQ, 0, sizeof(m_SEQ));
	memset(m_FCS, 0, sizeof(m_FCS));

	//
	memset(m_ErroeReportNums, 0, sizeof(m_ErroeReportNums));
	memset(m_ErrorReportId, 0, sizeof(m_ErrorReportId));
	memset(m_ErrorType, 0, sizeof(m_ErrorType));
	memset(m_OccurTime, 0, sizeof(m_OccurTime));
	memset(m_RecoverTime, 0, sizeof(m_RecoverTime));
	memset(m_ErrorTypeNameLen, 0, sizeof(m_ErrorTypeNameLen));
	memset(m_ErrorTypeName, 0, sizeof(m_ErrorTypeName));
	memset(m_ErrorDesLen, 0, sizeof(m_ErrorDesLen));
	memset(m_ErrorDes, 0, sizeof(m_ErrorDes));

}

bool CLocalFault::CheckCommand(char* data)
{
	if (!data)
	{
		return false;
	}
	else
	{
		Clear();
	}

	int palce = 0;

	//帧头标志HEAD
	memcpy(m_HEAD, data, sizeof(m_HEAD)-1);
	if (strcmp(m_HEAD, "TXKZQ1"))
	{
		return false;
	}
	palce += sizeof(m_HEAD) -1;

	//信宿标识DA
	memcpy(m_DA, data+palce, sizeof(m_DA)-1);
	palce += sizeof(m_DA) -1;

	//信源标识SA
	memcpy(m_SA, data+palce, sizeof(m_SA)-1);
	palce += sizeof(m_SA) -1;

	//帧长度 LEN
	memcpy(m_LEN, data+palce, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN) -1;

	//帧命令类型 FT
	memcpy(m_FT, data+palce, sizeof(m_FT)-1);
	palce += sizeof(m_FT) -1;

	//帧流水号 SEQ
	memcpy(m_SEQ, data+palce, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ) -1;

	//帧校验 FCS
	memcpy(m_FCS, data+palce, sizeof(m_FCS)-1);

	return true;
}



bool CLocalFault::Response(int flowNumber)
{
	//组装结构包
	int palce = 0;
	int checkPlace = 0;
	int signPlace = 0;   //?
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));

	//帧头标志HEAD
	memcpy(m_szResponsePackage, m_HEAD, sizeof(m_HEAD)-1);
	palce += sizeof(m_HEAD)-1;

	//信宿标识DA
	memcpy(m_szResponsePackage+palce, m_SA, sizeof(m_SA)-1);
	palce += sizeof(m_SA)-1;

	//信源标识SA
	memcpy(m_szResponsePackage+palce, m_DA, sizeof(m_DA)-1);
	palce += sizeof(m_DA)-1;
	checkPlace = palce;

	//帧长度 LEN
	memset(m_LEN, 0, sizeof(m_LEN));
	if (!YCodec::IntToBytes(35, m_LEN, sizeof(m_LEN)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_LEN, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN)-1;

	//帧命令类型 FT
	memset(m_FT, 0, sizeof(m_FT));
	YCodec::HexString2Bytes("0x85", m_FT, sizeof(m_FT)-1);
	memcpy(m_szResponsePackage+palce, m_FT, sizeof(m_FT)-1);
	palce += sizeof(m_FT)-1;

	//帧流水号 SEQ
	memset(m_SEQ, 0, sizeof(m_SEQ));
	if (!YCodec::IntToBytes(flowNumber, m_SEQ, sizeof(m_SEQ)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_SEQ, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ)-1;

	//故障信息(从数据库中取)
	string strError;
	std::list<LOCALFAULT> localfaultinfo;
	if (!CDb::getLocalFault(localfaultinfo, strError))
	{
		return false;
	}

	//遍历所有的vpx
	int strLength = 0;
	list<LOCALFAULT>::iterator it;
	for (it = localfaultinfo.begin(); it != localfaultinfo.end(); it++)
	{
		/*寻找本地数据
		if (!it->bIsSelf)
		{
			continue;
		}
		*/
		//本次故障告警记录数量M
		if(!LocalFauNums(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_ErroeReportNums, sizeof(m_ErroeReportNums)-1);
		palce += sizeof(m_ErroeReportNums)-1;
		
		//故障告警记录ID
		if(!FaultRecoId(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_ErrorReportId, sizeof(m_ErrorReportId)-1);
		palce += sizeof(m_ErrorReportId)-1;
	
		//故障告警类型编码
		if(!FaultTypeCode(*it))
		{
			return false;

		}
		memcpy(m_szResponsePackage+palce, m_ErrorType, sizeof(m_ErrorType)-1);
		palce += sizeof(m_ErrorType)-1;
		
		//发生时间
		if(!FaultTime(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_OccurTime, sizeof(m_OccurTime)-1);
		palce += sizeof(m_OccurTime)-1;

		//恢复时间
		if(!FaultRecoTime(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_RecoverTime, sizeof(m_RecoverTime)-1);
		palce += sizeof(m_RecoverTime)-1;
		
		//告警类型名称长度
		if(!WarnTypeLen(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_ErrorTypeNameLen, sizeof(m_ErrorTypeNameLen)-1);
		palce += sizeof(m_ErrorTypeNameLen)-1;

		//告警类型名称
		if(!WarnTypeName(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_ErrorTypeName, sizeof(m_ErrorTypeName)-1);
		palce += sizeof(m_ErrorTypeName)-1;

		//告警描述长度
		if(!WarnDescLen(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_ErrorDesLen, sizeof(m_ErrorDesLen)-1);
		palce += sizeof(m_ErrorDesLen)-1;

		//告警描述
		if(!WarnDescri(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_ErrorDes, sizeof(m_ErrorDes)-1);
		palce += sizeof(m_ErrorDes)-1;

		//清空缓存
		localfaultinfo.clear();

		//重新写帧长度 LEN
		memset(m_LEN, 0, sizeof(m_LEN));
		if (!YCodec::IntToBytes(palce,m_LEN,sizeof(m_LEN)-1))
		{
			return false;
		}
		memcpy(m_szResponsePackage+signPlace, m_LEN, sizeof(m_LEN)-1);

		//帧校验 FCS
		short int nfce = YCodec::crc_ccitt(m_szResponsePackage+(sizeof(m_HEAD)-1)+(sizeof(m_DA)-1), palce-(sizeof(m_HEAD)-1)-(sizeof(m_DA)-1));
		if (!YCodec::IntToBytes(nfce,m_FCS,sizeof(m_FCS)-1))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_FCS, sizeof(m_FCS)-1);
		palce += sizeof(m_FCS)-1;

		m_nResponsePackageLen = palce;
		
		return true;
		
	}
}

bool CLocalFault::LocalFauNums(LOCALFAULT localfauinfo)
{
	memset(m_ErroeReportNums,0,sizeof(m_ErroeReportNums));
	if (!YCodec::IntToBytes(localfauinfo.m_faultAlarmRecord, m_ErroeReportNums, sizeof(m_ErroeReportNums)-1))
	{
		return false;
	}
	return true;
}

bool CLocalFault::FaultRecoId(LOCALFAULT localfauinfo)
{
	memset(m_ErrorReportId, 0 ,sizeof(m_ErrorReportId));
	if (!YCodec::IntToBytes(localfauinfo.m_faultAlarmId,m_ErrorReportId,sizeof(m_ErrorReportId)-1))
	{
		return false;
	}
	return true;
}

bool CLocalFault::FaultTypeCode(LOCALFAULT localfauinfo)
{
	memset(m_ErrorType, 0 ,sizeof(m_ErrorType));
	if (!YCodec::IntToBytes(localfauinfo.m_faultAlarmCode,m_ErrorType,sizeof(m_ErrorType)-1))
	{
		return false;
	}
	return true;
}

bool CLocalFault::FaultTime(LOCALFAULT localfauinfo)
{
	memset(m_OccurTime, 0, sizeof(m_OccurTime));
	int strLength = localfauinfo.m_faultStartTime.length() > sizeof(m_OccurTime) -1 ? sizeof(m_OccurTime) -1:localfauinfo.m_faultStartTime.length();
	memcpy(m_OccurTime, localfauinfo.m_faultStartTime.c_str(), strLength);
	return true;
}

bool CLocalFault::FaultRecoTime(LOCALFAULT localfauinfo)
{
	memset(m_RecoverTime, 0, sizeof(m_RecoverTime));
	int strLength = localfauinfo.m_faultRecoverTime.length() > sizeof(m_RecoverTime) -1 ? sizeof(m_RecoverTime) -1:localfauinfo.m_faultRecoverTime.length();
	memcpy(m_RecoverTime, localfauinfo.m_faultRecoverTime.c_str(), strLength);
	return true;
}

bool CLocalFault::WarnTypeLen(LOCALFAULT localfauinfo)
{
	memset(m_ErrorTypeNameLen,0,sizeof(m_ErrorTypeNameLen));
	if (!YCodec::IntToBytes(localfauinfo.m_faultNameLen, m_ErrorTypeNameLen, sizeof(m_ErrorTypeNameLen)-1))
	{
		return false;
	}
	return true;
}

bool CLocalFault::WarnTypeName(LOCALFAULT localfauinfo)
{
	memset(m_ErrorTypeName, 0, sizeof(m_ErrorTypeName));
	int strLength = localfauinfo.m_faultAlarmName.length() > sizeof(m_ErrorTypeName) -1 ? sizeof(m_ErrorTypeName) -1:localfauinfo.m_faultAlarmName.length();
	memcpy(m_ErrorTypeName, localfauinfo.m_faultAlarmName.c_str(), strLength);
	return true;
}

bool CLocalFault::WarnDescLen(LOCALFAULT localfauinfo)
{
	memset(m_ErrorDesLen,0,sizeof(m_ErrorDesLen));
	if (!YCodec::IntToBytes(localfauinfo.m_faultAlarmLen, m_ErrorDesLen, sizeof(m_ErrorDesLen)-1))
	{
		return false;
	}
	return true;
}

bool CLocalFault::WarnDescri(LOCALFAULT localfauinfo)
{
	memset(m_ErrorDes, 0, sizeof(m_ErrorDes));
	int strLength = localfauinfo.m_faultAlarmDescri.length() > sizeof(m_ErrorDes) -1 ? sizeof(m_ErrorDes) -1:localfauinfo.m_faultAlarmDescri.length();
	memcpy(m_ErrorDes, localfauinfo.m_faultAlarmDescri.c_str(), strLength);
	return true;
}

//char int转byte
void CLocalFault::charIntToByte(int i, char *bytes)
{
	//byte[] bytes = new byte[2];
	memset(bytes, 0, sizeof(char)*2);
	bytes[0] = (char) (0xff & i);
}