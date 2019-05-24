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
	//Ӧ��
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

	//֡ͷ��־HEAD
	memcpy(m_HEAD, data, sizeof(m_HEAD)-1);
	if (strcmp(m_HEAD, "TXKZQ1"))
	{
		return false;
	}
	palce += sizeof(m_HEAD) -1;

	//���ޱ�ʶDA
	memcpy(m_DA, data+palce, sizeof(m_DA)-1);
	palce += sizeof(m_DA) -1;

	//��Դ��ʶSA
	memcpy(m_SA, data+palce, sizeof(m_SA)-1);
	palce += sizeof(m_SA) -1;

	//֡���� LEN
	memcpy(m_LEN, data+palce, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN) -1;

	//֡�������� FT
	memcpy(m_FT, data+palce, sizeof(m_FT)-1);
	palce += sizeof(m_FT) -1;

	//֡��ˮ�� SEQ
	memcpy(m_SEQ, data+palce, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ) -1;

	//֡У�� FCS
	memcpy(m_FCS, data+palce, sizeof(m_FCS)-1);
	palce += sizeof(m_FCS)-1;

	return true;
}

bool CLocalControllerList::Response(int flowNumber)
{
	Clear();
	ClearResponse();

	//��װ�ṹ��
	int palce = 0;
	int signPlace = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));

	//֡ͷ��־HEAD
	memcpy(m_szResponsePackage+palce, m_HEAD, sizeof(m_HEAD)-1);
	palce += sizeof(m_HEAD)-1;

	//���ޱ�ʶDA
	memcpy(m_szResponsePackage+palce, m_SA, sizeof(m_SA)-1);
	palce += sizeof(m_SA)-1;

	//��Դ��ʶSA
	memcpy(m_szResponsePackage+palce, m_DA, sizeof(m_DA)-1);
	palce += sizeof(m_DA)-1;

	//֡���� LEN
	//��ǳ���Ԥ��λ
	signPlace = palce;
	memcpy(m_szResponsePackage+palce, m_LEN, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN)-1;

	//֡�������� FT
	memset(m_FT, 0, sizeof(m_FT));
	YCodec::HexString2Bytes("0x82", m_FT, sizeof(m_FT)-1);
	memcpy(m_szResponsePackage+palce, m_FT, sizeof(m_FT)-1);
	palce += sizeof(m_FT)-1;

	//֡��ˮ�� SEQ
	memset(m_SEQ, 0, sizeof(m_SEQ));
	if (!YCodec::IntToBytes(flowNumber,m_SEQ,sizeof(m_SEQ)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_SEQ, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ)-1;

	//ͨ�����б���Ϣ
	string strError;
	std::list<DEVICE> listRecords;
	if (!GetGECO(listRecords, strError))
	{
		return false;
	}

	//ͨ��������M
	if (!YCodec::IntToBytes(listRecords.size(),m_GECOCONT,sizeof(m_GECOCONT)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_GECOCONT, sizeof(m_GECOCONT)-1);
	palce += sizeof(m_GECOCONT)-1;

	//�������е�listm_GECOCONT
	int strLength = 0;
	list<DEVICE>::iterator it;
	for (it = listRecords.begin(); it != listRecords.end(); it++)
	{
		//ͨ����ID
		if (!AddGECOID(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_GECOID, sizeof(m_GECOID)-1);
		palce += sizeof(m_GECOID)-1;

		//ͨ��������
		if (!AddGECONAME(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_GECONAME, sizeof(m_GECONAME)-1);
		palce += sizeof(m_GECONAME)-1;

		//�豸����
		if (!AddEQCODING(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_EQCODING, sizeof(m_EQCODING)-1);
		palce += sizeof(m_EQCODING)-1;

		//Ӳ���汾
		if (!AddHARDWARE(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_HARDWARE, sizeof(m_HARDWARE)-1);
		palce += sizeof(m_HARDWARE)-1;

		//����汾
		if (!AddSOFTWAR(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_SOFTWARE, sizeof(m_SOFTWARE)-1);
		palce += sizeof(m_SOFTWARE)-1;
		
		//����IP��ַ
		if (!AddNETWORKIP(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_NETWORKIP, sizeof(m_NETWORKIP)-1);
		palce += sizeof(m_NETWORKIP)-1;

		//����IP��ַ
		if (!AddMANAGEMENTIP(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_MANAGEMENTIP, sizeof(m_MANAGEMENTIP)-1);
		palce += sizeof(m_MANAGEMENTIP)-1;

		//�豸״̬
		if (!AddEQUIPMENTSTATE(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_EQUIPMENTSTATE, sizeof(m_EQUIPMENTSTATE)-1);
		palce += sizeof(m_EQUIPMENTSTATE)-1;

		//����
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

		//γ��
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

		//����������
		if (!AddCONTROLLERLEVEL())
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_CONTROLLERLEVEL, sizeof(m_CONTROLLERLEVEL)-1);
		palce += sizeof(m_CONTROLLERLEVEL)-1;
	}
	
	//��ջ���
	listRecords.clear();

	//����д֡���� LEN
	memset(m_LEN, 0, sizeof(m_LEN));
	if (!YCodec::IntToBytes(palce,m_LEN,sizeof(m_LEN)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+signPlace, m_LEN, sizeof(m_LEN)-1);

	//֡У�� FCS
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


//��ȡͨ����
bool CLocalControllerList::GetGECO(list<DEVICE> &lstDevs, string &strError)
{
	if (!CDb::getSearchDevs(lstDevs, strError))
	{
		return false;
	}
	return true;
}

//ͨ����ID
bool CLocalControllerList::AddGECOID(DEVICE device)
{
	memset(m_GECOID, 0 ,sizeof(m_GECOID));
	if (!YCodec::IntToBytes(device.m_nId,m_GECOID,sizeof(m_GECOID)-1))
	{
		return false;
	}
	return true;
}

//ͨ��������
bool CLocalControllerList::AddGECONAME(DEVICE device)
{
	memset(m_GECONAME, 0, sizeof(m_GECONAME));
	int strLength = device.m_strName.length() > sizeof(m_GECONAME) -1 ? sizeof(m_GECONAME) -1:device.m_strName.length();
	memcpy(m_GECONAME, device.m_strName.c_str(), strLength);
	return true;
}

//�豸����
bool CLocalControllerList::AddEQCODING(DEVICE device)
{
	memset(m_EQCODING, 0, sizeof(m_EQCODING));
	int strLength = device.m_strNumber.length() > sizeof(m_EQCODING) -1 ? sizeof(m_EQCODING) -1:device.m_strNumber.length();
	memcpy(m_EQCODING, device.m_strNumber.c_str(), strLength);
	return true;
}

//Ӳ���汾
bool CLocalControllerList::AddHARDWARE(DEVICE device)
{
	memset(m_HARDWARE, 0, sizeof(m_HARDWARE));
	YCodec::HexString2Bytes("0x01000000", m_HARDWARE, sizeof(m_HARDWARE)-1);
	return true;
}

//����汾
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

//����IP��ַ
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

//����IP��ַ
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

//�豸״̬
bool CLocalControllerList::AddEQUIPMENTSTATE(DEVICE device)
{
	//�豸״̬
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

//�����ַ�(�ȡ��֡���)
bool CLocalControllerList::AddLONGITUDECHAR(DEVICE device)
{
	memset(m_LONGITUDECHAR, 0, sizeof(m_LONGITUDECHAR));
	//memset(m_LONGITUDEVALUE, 0, sizeof(m_LONGITUDEVALUE));
	memset(m_LONGITUDEDEGRR, 0, sizeof(m_LONGITUDEDEGRR));
	memset(m_LONGITUDEPOINT, 0, sizeof(m_LONGITUDEPOINT));
	memset(m_LONGITUDESECONDS, 0, sizeof(m_LONGITUDESECONDS));

	//����
	memcpy(m_LONGITUDECHAR, device.m_strLongittude.c_str(), sizeof(m_LONGITUDECHAR)-1);

	//���ַ�������ȡ��ĸ
	//float temp = 0.0;
	int index = 1;
	list<int> nums = YCodec::GetStringByNumArr(device.m_strLongittude);

	//�ȡ��֡���
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

	//������ת�����ַ���
	//YCodec::num2char(m_LONGITUDEVALUE,temp,3,4);
	return true;
}

//�����ַ�(�ȡ��֡���) 
bool CLocalControllerList::AddLATITUDECHAR(DEVICE device)
{
	memset(m_LATITUDECHAR, 0, sizeof(m_LATITUDECHAR));
	//memset(m_LATITUDEVALUE, 0, sizeof(m_LATITUDEVALUE));
	memset(m_LATITUDEDEGRR, 0, sizeof(m_LATITUDEDEGRR));
	memset(m_LATITUDEPOINT, 0, sizeof(m_LATITUDEPOINT));
	memset(m_LATITUDESECONDS, 0, sizeof(m_LATITUDESECONDS));

	//����
	memcpy(m_LATITUDECHAR, device.m_strLatitude.c_str(), sizeof(m_LATITUDECHAR)-1);

	//���ַ�������ȡ��ĸ
	//float temp = 0.0;
	int index = 1;
	list<int> nums = YCodec::GetStringByNumArr(device.m_strLatitude);

	//�ȡ��֡���
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

	//������ת�����ַ���
	//YCodec::num2char(m_LONGITUDEVALUE,temp,2,4);

	return true;
}


//����������
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
//��������
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
