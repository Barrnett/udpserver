#include "LocalControllerList.h"
#include <assert.h>
#include <memory.h>
#include <string.h>
#include "MxHandlePool.h"
#include "../MXDatabase/NetElementDb.h"
#include "../Tool/YCodec.h"
#include "../Tool/Tools.h"
using namespace Tools;


CLocalControllerList::CLocalControllerList()
{
	Clear();
	ClearCommand();
	ClearResponse();
}

CLocalControllerList::~CLocalControllerList()
{
	Clear();
	ClearCommand();
	ClearResponse();
}

void CLocalControllerList::Clear()
{
	m_nResponsePackageLen = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));

}

void CLocalControllerList::ClearCommand()
{
	memset(m_HEAD, 0, sizeof(m_HEAD));
	memset(m_DA, 0, sizeof(m_DA));
	memset(m_SA, 0, sizeof(m_SA));
	memset(m_LEN, 0, sizeof(m_LEN));
	memset(m_FT, 0, sizeof(m_FT));
	memset(m_SEQ, 0, sizeof(m_SEQ));
	memset(m_FCS, 0, sizeof(m_FCS));
}
void CLocalControllerList::ClearResponse()
{
	//应答
	memset(m_GECOCONT, 0, sizeof(m_GECOCONT));
	memset(m_GECOID, 0, sizeof(m_GECOID));
	memset(m_GECONAME, 0, sizeof(m_GECONAME));
	memset(m_EQCODING, 0, sizeof(m_EQCODING));
	memset(m_HARDWARE, 0, sizeof(m_HARDWARE));
	memset(m_SOFTWARE, 0, sizeof(m_SOFTWARE));
	memset(m_NETWORKIP, 0, sizeof(m_NETWORKIP));
	memset(m_MANAGEMENTIP, 0, sizeof(m_MANAGEMENTIP));
	memset(m_EQUIPMENTSTATE, 0, sizeof(m_EQUIPMENTSTATE));
	memset(m_LONGITUDECHAR, 0, sizeof(m_LONGITUDECHAR));
	//memset(m_LONGITUDEVALUE, 0, sizeof(m_LONGITUDEVALUE));
	memset(m_LONGITUDEDEGRR, 0, sizeof(m_LONGITUDEDEGRR));
	memset(m_LONGITUDEPOINT, 0, sizeof(m_LONGITUDEPOINT));
	memset(m_LONGITUDESECONDS, 0, sizeof(m_LONGITUDESECONDS));
	memset(m_LATITUDECHAR, 0, sizeof(m_LATITUDECHAR));
	//memset(m_LATITUDEVALUE, 0, sizeof(m_LATITUDEVALUE));
	memset(m_LATITUDEDEGRR, 0, sizeof(m_LATITUDEDEGRR));
	memset(m_LATITUDEPOINT, 0, sizeof(m_LATITUDEPOINT));
	memset(m_LATITUDESECONDS, 0, sizeof(m_LATITUDESECONDS));
	memset(m_CONTROLLERLEVEL, 0, sizeof(m_CONTROLLERLEVEL));
}



bool CLocalControllerList::CheckCommand(char* data)
{
	if (!data)
	{
		return false;
	}
	else
	{
		ClearCommand();
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
	palce += sizeof(m_FCS)-1;

	return true;
}

bool CLocalControllerList::Response(int flowNumber)
{
	Clear();
	ClearResponse();

	//组装结构包
	int palce = 0;
	int signPlace = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));

	//帧头标志HEAD
	memcpy(m_szResponsePackage+palce, m_HEAD, sizeof(m_HEAD)-1);
	palce += sizeof(m_HEAD)-1;

	//信宿标识DA
	memcpy(m_szResponsePackage+palce, m_SA, sizeof(m_SA)-1);
	palce += sizeof(m_SA)-1;

	//信源标识SA
	memcpy(m_szResponsePackage+palce, m_DA, sizeof(m_DA)-1);
	palce += sizeof(m_DA)-1;

	//帧长度 LEN
	//标记长度预留位
	signPlace = palce;
	memcpy(m_szResponsePackage+palce, m_LEN, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN)-1;

	//帧命令类型 FT
	memset(m_FT, 0, sizeof(m_FT));
	YCodec::HexString2Bytes("0x82", m_FT, sizeof(m_FT)-1);
	memcpy(m_szResponsePackage+palce, m_FT, sizeof(m_FT)-1);
	palce += sizeof(m_FT)-1;

	//帧流水号 SEQ
	memset(m_SEQ, 0, sizeof(m_SEQ));
	if (!YCodec::IntToBytes(flowNumber,m_SEQ,sizeof(m_SEQ)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_SEQ, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ)-1;

	//通控器列表信息
	string strError;
	std::list<DEVICE> listRecords;
	if (!GetGECO(listRecords, strError))
	{
		return false;
	}

	//通控器总数M
	if (!YCodec::IntToBytes(listRecords.size(),m_GECOCONT,sizeof(m_GECOCONT)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_GECOCONT, sizeof(m_GECOCONT)-1);
	palce += sizeof(m_GECOCONT)-1;

	//遍历所有的listm_GECOCONT
	int strLength = 0;
	list<DEVICE>::iterator it;
	for (it = listRecords.begin(); it != listRecords.end(); it++)
	{
		//通控器ID
		if (!AddGECOID(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_GECOID, sizeof(m_GECOID)-1);
		palce += sizeof(m_GECOID)-1;

		//通控器名称
		if (!AddGECONAME(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_GECONAME, sizeof(m_GECONAME)-1);
		palce += sizeof(m_GECONAME)-1;

		//设备编码
		if (!AddEQCODING(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_EQCODING, sizeof(m_EQCODING)-1);
		palce += sizeof(m_EQCODING)-1;

		//硬件版本
		if (!AddHARDWARE(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_HARDWARE, sizeof(m_HARDWARE)-1);
		palce += sizeof(m_HARDWARE)-1;

		//软件版本
		if (!AddSOFTWAR(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_SOFTWARE, sizeof(m_SOFTWARE)-1);
		palce += sizeof(m_SOFTWARE)-1;
		
		//网络IP地址
		if (!AddNETWORKIP(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_NETWORKIP, sizeof(m_NETWORKIP)-1);
		palce += sizeof(m_NETWORKIP)-1;

		//管理IP地址
		if (!AddMANAGEMENTIP(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_MANAGEMENTIP, sizeof(m_MANAGEMENTIP)-1);
		palce += sizeof(m_MANAGEMENTIP)-1;

		//设备状态
		if (!AddEQUIPMENTSTATE(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_EQUIPMENTSTATE, sizeof(m_EQUIPMENTSTATE)-1);
		palce += sizeof(m_EQUIPMENTSTATE)-1;

		//经度
		if (!AddLONGITUDECHAR(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_LONGITUDECHAR, sizeof(m_LONGITUDECHAR)-1);
		palce += sizeof(m_LONGITUDECHAR)-1;
		//memcpy(m_szResponsePackage+palce, m_LONGITUDEVALUE, sizeof(m_LONGITUDEVALUE)-1);
		//palce += sizeof(m_LONGITUDEVALUE)-1;
		memcpy(m_szResponsePackage+palce, m_LONGITUDEDEGRR, sizeof(m_LONGITUDEDEGRR)-1);
		palce += sizeof(m_LONGITUDEDEGRR)-1;
		memcpy(m_szResponsePackage+palce, m_LONGITUDEPOINT, sizeof(m_LONGITUDEPOINT)-1);
		palce += sizeof(m_LONGITUDEPOINT)-1;
		memcpy(m_szResponsePackage+palce, m_LONGITUDESECONDS, sizeof(m_LONGITUDESECONDS)-1);
		palce += sizeof(m_LONGITUDESECONDS)-1;

		//纬度
		if (!AddLATITUDECHAR(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_LATITUDECHAR, sizeof(m_LATITUDECHAR)-1);
		palce += sizeof(m_LATITUDECHAR)-1;
		//memcpy(m_szResponsePackage+palce, m_LATITUDEVALUE, sizeof(m_LATITUDEVALUE)-1);
		//palce += sizeof(m_LATITUDEVALUE)-1;
		memcpy(m_szResponsePackage+palce, m_LATITUDEDEGRR, sizeof(m_LATITUDEDEGRR)-1);
		palce += sizeof(m_LATITUDEDEGRR)-1;

		memcpy(m_szResponsePackage+palce, m_LATITUDEPOINT, sizeof(m_LATITUDEPOINT)-1);
		palce += sizeof(m_LATITUDEPOINT)-1;

		memcpy(m_szResponsePackage+palce, m_LATITUDESECONDS, sizeof(m_LATITUDESECONDS)-1);
		palce += sizeof(m_LATITUDESECONDS)-1;

		//控制器级别
		if (!AddCONTROLLERLEVEL())
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_CONTROLLERLEVEL, sizeof(m_CONTROLLERLEVEL)-1);
		palce += sizeof(m_CONTROLLERLEVEL)-1;
	}
	
	//清空缓存
	listRecords.clear();

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


//获取通控器
bool CLocalControllerList::GetGECO(list<DEVICE> &lstDevs, string &strError)
{
	if (!CDb::getSearchDevs(lstDevs, strError))
	{
		return false;
	}
	return true;
}

//通控器ID
bool CLocalControllerList::AddGECOID(DEVICE device)
{
	memset(m_GECOID, 0 ,sizeof(m_GECOID));
	if (!YCodec::IntToBytes(device.m_nId,m_GECOID,sizeof(m_GECOID)-1))
	{
		return false;
	}
	return true;
}

//通控器名称
bool CLocalControllerList::AddGECONAME(DEVICE device)
{
	memset(m_GECONAME, 0, sizeof(m_GECONAME));
	int strLength = device.m_strName.length() > sizeof(m_GECONAME) -1 ? sizeof(m_GECONAME) -1:device.m_strName.length();
	memcpy(m_GECONAME, device.m_strName.c_str(), strLength);
	return true;
}

//设备编码
bool CLocalControllerList::AddEQCODING(DEVICE device)
{
	memset(m_EQCODING, 0, sizeof(m_EQCODING));
	int strLength = device.m_strNumber.length() > sizeof(m_EQCODING) -1 ? sizeof(m_EQCODING) -1:device.m_strNumber.length();
	memcpy(m_EQCODING, device.m_strNumber.c_str(), strLength);
	return true;
}

//硬件版本
bool CLocalControllerList::AddHARDWARE(DEVICE device)
{
	memset(m_HARDWARE, 0, sizeof(m_HARDWARE));
	YCodec::HexString2Bytes("0x01000000", m_HARDWARE, sizeof(m_HARDWARE)-1);
	return true;
}

//软件版本
bool CLocalControllerList::AddSOFTWAR(DEVICE device)
{
	memset(m_SOFTWARE, 0, sizeof(m_SOFTWARE));
	int index = 0;
	list<int> nums = YCodec::GetStringByNumArr(device.m_strSoftVersion);
	for (list<int>::iterator it = nums.begin(); it != nums.end(); it++)
	{
		m_SOFTWARE[index++] = *it;
	}
	return true;
}

//网络IP地址
bool CLocalControllerList::AddNETWORKIP(DEVICE device)
{
	memset(m_NETWORKIP,0,sizeof(m_NETWORKIP));
	unsigned int nInterfaceIp = Tools::string_to_unsigned_int(device.m_strBoardIp);
	if (!YCodec::IntToBytes_Network(nInterfaceIp,m_NETWORKIP,sizeof(m_NETWORKIP)-1))
	{
		return false;
	}
	return true;
}

//管理IP地址
bool CLocalControllerList::AddMANAGEMENTIP(DEVICE device)
{
	memset(m_MANAGEMENTIP,0,sizeof(m_MANAGEMENTIP));
	unsigned int nInterfaceIp = Tools::string_to_unsigned_int(device.m_strManageIp);

	if (!YCodec::IntToBytes_Network(nInterfaceIp,m_MANAGEMENTIP,sizeof(m_MANAGEMENTIP)-1))
	{
		return false;
	}
	return true;
}

//设备状态
bool CLocalControllerList::AddEQUIPMENTSTATE(DEVICE device)
{
	//设备状态
	memset(m_EQUIPMENTSTATE, 0, sizeof(m_EQUIPMENTSTATE));
	if (0 == device.m_nIsActivity)
	{
		YCodec::HexString2Bytes("0x01010101", m_EQUIPMENTSTATE, sizeof(m_EQUIPMENTSTATE)-1);
	}
	else
	{
		YCodec::HexString2Bytes("0x00000000", m_EQUIPMENTSTATE, sizeof(m_EQUIPMENTSTATE)-1);

		/*
		if (!YCodec::IntToBytes(device.m_nState,m_EQUIPMENTSTATE,sizeof(m_EQUIPMENTSTATE)-1))
		{
			return false;
		}
		*/
	}
	return true;
}

//经度字符(度、分、秒)
bool CLocalControllerList::AddLONGITUDECHAR(DEVICE device)
{
	memset(m_LONGITUDECHAR, 0, sizeof(m_LONGITUDECHAR));
	//memset(m_LONGITUDEVALUE, 0, sizeof(m_LONGITUDEVALUE));
	memset(m_LONGITUDEDEGRR, 0, sizeof(m_LONGITUDEDEGRR));
	memset(m_LONGITUDEPOINT, 0, sizeof(m_LONGITUDEPOINT));
	memset(m_LONGITUDESECONDS, 0, sizeof(m_LONGITUDESECONDS));

	//经度
	memcpy(m_LONGITUDECHAR, device.m_strLongittude.c_str(), sizeof(m_LONGITUDECHAR)-1);

	//从字符串里提取字母
	//float temp = 0.0;
	int index = 1;
	list<int> nums = YCodec::GetStringByNumArr(device.m_strLongittude);

	//度、分、秒
	for (list<int>::iterator it = nums.begin(); it != nums.end(); it++)
	{
		switch (index++)
		{
		case 1:
			{
				if (!YCodec::IntToBytes(*it,m_LONGITUDEDEGRR,sizeof(m_LONGITUDEDEGRR)-1))
				{
					return false;
				}
				break;
			}
		case 2:
			{
				if (!YCodec::IntToBytes(*it,m_LONGITUDEPOINT,sizeof(m_LONGITUDEPOINT)-1))
				{
					return false;
				}
				break;
			}
		case 3:
			{
				if (!YCodec::IntToBytes(*it,m_LONGITUDESECONDS,sizeof(m_LONGITUDESECONDS)-1))
				{
					return false;
				}
				break;
			}
		}
	}

	//浮点数转换成字符串
	//YCodec::num2char(m_LONGITUDEVALUE,temp,3,4);
	return true;
}

//经度字符(度、分、秒) 
bool CLocalControllerList::AddLATITUDECHAR(DEVICE device)
{
	memset(m_LATITUDECHAR, 0, sizeof(m_LATITUDECHAR));
	//memset(m_LATITUDEVALUE, 0, sizeof(m_LATITUDEVALUE));
	memset(m_LATITUDEDEGRR, 0, sizeof(m_LATITUDEDEGRR));
	memset(m_LATITUDEPOINT, 0, sizeof(m_LATITUDEPOINT));
	memset(m_LATITUDESECONDS, 0, sizeof(m_LATITUDESECONDS));

	//经度
	memcpy(m_LATITUDECHAR, device.m_strLatitude.c_str(), sizeof(m_LATITUDECHAR)-1);

	//从字符串里提取字母
	//float temp = 0.0;
	int index = 1;
	list<int> nums = YCodec::GetStringByNumArr(device.m_strLatitude);

	//度、分、秒
	for (list<int>::iterator it = nums.begin(); it != nums.end(); it++)
	{
		switch (index++)
		{
		case 1:
			{
				if (!YCodec::IntToBytes(*it,m_LATITUDEDEGRR,sizeof(m_LATITUDEDEGRR)-1))
				{
					return false;
				}
				break;
			}
		case 2:
			{
				if (!YCodec::IntToBytes(*it,m_LATITUDEPOINT,sizeof(m_LATITUDEPOINT)-1))
				{
					return false;
				}
				break;
			}
		case 3:
			{
				if (!YCodec::IntToBytes(*it,m_LATITUDESECONDS,sizeof(m_LATITUDESECONDS)-1))
				{
					return false;
				}
				break;
			}
		}
	}

	//浮点数转换成字符串
	//YCodec::num2char(m_LONGITUDEVALUE,temp,2,4);

	return true;
}


//控制器级别
bool CLocalControllerList::AddCONTROLLERLEVEL()
{
	memset(m_CONTROLLERLEVEL, 0, sizeof(m_CONTROLLERLEVEL));
	charIntToByte(1,m_CONTROLLERLEVEL);
	/*
	memcpy(m_CONTROLLERLEVEL, "1", sizeof(m_CONTROLLERLEVEL)-1);
	if (!YCodec::IntToBytes(1,m_CONTROLLERLEVEL,sizeof(m_CONTROLLERLEVEL)-1))
	{
		return false;
	}
	*/
	return true;
}

void CLocalControllerList::charIntToByte(int i, char *bytes)
{
	//byte[] bytes = new byte[2];
	memset(bytes, 0, sizeof(char)*2);
	bytes[0] = (char) (0xff & i);
}


/*
//拷贝函数
void* CLocalControllerList::MyMemMove(void *dst, const void *src, int count) 
{ 
	assert(dst); 
	assert(src); 
	void *ret=dst; 
	while (count--) 
	{ 
		*(char *)dst = *(char *)src; 
		dst = (char *)dst + 1; 
		src = (char *)src + 1; 
	} 
	return ret; 
}
*/
