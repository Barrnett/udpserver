#include <assert.h>
#include <memory.h>
#include <string.h>
#include "NetworkInformation.h"
#include "../Tool/YCodec.h"
#include <cmath>
#include "../Tool/Tools.h"
using namespace Tools;


CNetworkInformation::CNetworkInformation()
{
	Clear();
	ClearCommand();
	ClearResponse();
}

CNetworkInformation::~CNetworkInformation()
{
	Clear();
	ClearCommand();
	ClearResponse();
}

void CNetworkInformation::ClearCommand()
{
	memset(m_HEAD, 0, sizeof(m_HEAD));
	memset(m_DA, 0, sizeof(m_DA));
	memset(m_SA, 0, sizeof(m_SA));
	memset(m_LEN, 0, sizeof(m_LEN));
	memset(m_FT, 0, sizeof(m_FT));
	memset(m_SEQ, 0, sizeof(m_SEQ));
	memset(m_MIMA, 0, sizeof(m_MIMA));
	memset(m_MAPA, 0, sizeof(m_MAPA));
	memset(m_QUPA, 0, sizeof(m_QUPA));
	memset(m_FCS, 0, sizeof(m_FCS));
}

void CNetworkInformation::ClearResponse()
{
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));
	//
	memset(m_ConfirmLogo, 0, sizeof(m_ConfirmLogo));
	memset(m_ReplyLogo, 0, sizeof(m_ReplyLogo));
	memset(m_TaskToTal, 0, sizeof(m_TaskToTal));
	memset(m_RelplySequence, 0, sizeof(m_RelplySequence));
	memset(m_LinkNumber, 0, sizeof(m_LinkNumber));
	memset(m_LinkState, 0, sizeof(m_LinkState));
	memset(m_LinkType, 0, sizeof(m_LinkType));
	memset(m_Bandwidth, 0, sizeof(m_Bandwidth));
	memset(m_BandwidthUtilization, 0, sizeof(m_BandwidthUtilization));
	memset(m_SrcNodeId, 0, sizeof(m_SrcNodeId));
	memset(m_StartInterfaceIp, 0, sizeof(m_StartInterfaceIp));
	memset(m_SrcNodeInterface, 0, sizeof(m_SrcNodeInterface));
	memset(m_StartStatus, 0, sizeof(m_StartStatus));
	memset(m_StartErrorRate, 0, sizeof(m_StartErrorRate));
	memset(m_StartJitter, 0, sizeof(m_StartJitter));
	memset(m_StartAvgrtt, 0, sizeof(m_StartAvgrtt));
	memset(m_StartPacketloss, 0, sizeof(m_StartPacketloss));
	memset(m_DstNodeId, 0, sizeof(m_DstNodeId));
	memset(m_EndInterfaceIp, 0, sizeof(m_EndInterfaceIp));
	memset(m_DstNodeInterface, 0, sizeof(m_DstNodeInterface));
	memset(m_EndStatus, 0, sizeof(m_EndStatus));
	memset(m_EndErrorRate, 0, sizeof(m_EndErrorRate));
	memset(m_EndJitter, 0, sizeof(m_EndJitter));
	memset(m_EndAvgrtt, 0, sizeof(m_EndAvgrtt));
	memset(m_EndPacketloss, 0, sizeof(m_EndPacketloss));
	//
	memset(m_ExpePack, 0, sizeof(m_ExpePack));
}


void CNetworkInformation::Clear()
{
	m_nSignPlace = 0;
	m_nFlowNumber = 0;
	m_nPacketOrder = 0;
	m_nResponsePackageLen = 0;
}


bool CNetworkInformation::CheckCommand(char* data)
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

	//��ѯ�����ʶ
	memcpy(m_MIMA, data+palce, sizeof(m_MIMA)-1);
	palce += sizeof(m_MIMA) -1;

	//������ݰ�����
	memcpy(m_MAPA, data+palce, sizeof(m_MAPA)-1);
	palce += sizeof(m_MAPA) -1;
	
	//���β�ѯ�����
	memcpy(m_QUPA, data+palce, sizeof(m_QUPA)-1);
	palce += sizeof(m_QUPA) -1;

	//֡У�� FCS
	memcpy(m_FCS, data+palce, sizeof(m_FCS)-1);

	return true;
}

//�ظ���
bool CNetworkInformation::Response(int flowNumber)
{
	Clear();
	ClearResponse();

	int packetOrder = 0;
	if (!YCodec::bytesToInt(m_QUPA, packetOrder, sizeof(m_QUPA)-1))
	{
		//��ȡ����ʧ��
		return false;
	}

	//����ȡ����
	if (packetOrder == 1)     //���»�ȡ��
	{
		if (!GetDataBase())
		{
			//��ȡ����ʧ��
			return false;
		}
		//����Ӧ��
		return ResponseNormal(packetOrder);
	}
	else if (packetOrder == m_nPacketOrder + 1)
	{
		//����Ӧ��
		return ResponseNormal(packetOrder);
	}
	else
	{
		//����Ӧ��
		return ResponseDeny(packetOrder);
	}
	return false;
}

//����Ӧ��
bool CNetworkInformation::ResponseNormal(int packetOrder)
{
	ClearResponse();

	int bufferLen = 0;
	int bufferTotalLen = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));
	//��װ��ͷ
	if (!FillHeader(m_szResponsePackage, bufferLen))
	{
		return false;
	}
	bufferTotalLen = bufferLen;
	//��װ������
	if (!FillHandle(m_szResponsePackage+bufferTotalLen, bufferLen))
	{
		return false;
	}
	bufferTotalLen += bufferLen;

	//1024�ֽڿ��ԷŶ��ٸ����ݰ���
	int nMaxNum = floor(float(1024/FillDataSize()));
	//������ʼ�±�
	int nIndex = (packetOrder-1)*nMaxNum;


	//��װ��������
	list<NETELEMENTLINE>::iterator iter = m_listLines.begin();
	for (int i = 0; i < nMaxNum, iter != m_listLines.end(); iter++)
	{
		if (i < nIndex)
		{
			continue;
		}
		if (!FillData(*iter, m_szResponsePackage+bufferTotalLen, bufferLen))
		{
			return false;
		}
		bufferTotalLen += bufferLen;
	}

	//��װ��֤
	if (!FillVerify(bufferTotalLen))
	{
		return false;
	}

	m_nResponsePackageLen = bufferTotalLen;
	return true;
}


//����Ӧ��
bool CNetworkInformation::ResponseDeny(int packetOrder)
{
	ClearResponse();

	//��װ�ṹ��
	int nPalce = 0;
	int nSignPlace = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));


	//֡ͷ��־HEAD
	memcpy(m_szResponsePackage, m_HEAD, sizeof(m_HEAD)-1);
	nPalce += sizeof(m_HEAD)-1;

	//���ޱ�ʶDA
	memcpy(m_szResponsePackage+nPalce, m_SA, sizeof(m_SA)-1);
	nPalce += sizeof(m_SA)-1;

	//��Դ��ʶSA
	memcpy(m_szResponsePackage+nPalce, m_DA, sizeof(m_DA)-1);
	nPalce += sizeof(m_DA)-1;
	nSignPlace = nPalce;

	//֡���� LEN
	memcpy(m_szResponsePackage+nPalce, m_LEN, sizeof(m_LEN)-1);
	nPalce += sizeof(m_LEN)-1;

	//֡�������� FT
	YCodec::HexString2Bytes("83", m_FT, sizeof(m_FT)-1);
	memcpy(m_szResponsePackage+nPalce, m_FT, sizeof(m_FT)-1);
	nPalce += sizeof(m_FT)-1;

	//֡��ˮ�� SEQ
	YCodec::intToBytes(m_nFlowNumber, m_SEQ);
	memcpy(m_szResponsePackage+nPalce, m_SEQ, sizeof(m_SEQ)-1);
	nPalce += sizeof(m_SEQ)-1;

	//ȷ�ϱ�ʶ
	if (!YCodec::IntToBytes(3, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nPalce, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1);
	nPalce += sizeof(m_ConfirmLogo)-1;

	//��ѯ�����ʶ
	memcpy(m_szResponsePackage+nPalce, m_MIMA, sizeof(m_MIMA)-1);
	nPalce += sizeof(m_ReplyLogo)-1;

	//�����İ����
	if (!YCodec::IntToBytes(packetOrder, m_ExpePack))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nPalce, m_ExpePack, sizeof(m_ExpePack)-1);
	nPalce += sizeof(m_ExpePack)-1;

	//����д֡���� LEN
	if (!YCodec::IntToBytes(nPalce,m_LEN,sizeof(m_LEN)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nSignPlace, m_LEN, sizeof(m_LEN)-1);

	//֡У�� FCS
	short int nfce = YCodec::crc_ccitt(m_szResponsePackage+(sizeof(m_HEAD)+1)-(sizeof(m_DA)+1), nPalce-(sizeof(m_HEAD)-1)-(sizeof(m_DA)-1));
	if (!YCodec::IntToBytes(nfce,m_FCS,sizeof(m_FCS)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nPalce, m_FCS, sizeof(m_FCS)-1);
	nPalce += sizeof(m_FCS)-1;
	m_nResponsePackageLen = nPalce;

	return true;
}


//��ȡ���ݿ�����
bool CNetworkInformation::GetDataBase()
{
	//ɾ����������
	m_listLines.clear();
	/*
	list<NETELEMENTLINE>::iterator iter;
	for (iter = m_listLines.begin(); iter != m_listLines.end(); iter++)
	{
		iter = m_listLines.erase(iter);
	}
	*/

	//��ȡ��������
	string strError;
	if (!CDb::getLines(m_listLines, strError))
	{
		return false;
	}
	return true;
}


//������ݰ���С
int CNetworkInformation::FillDataSize()
{
	int palce = 0;
	palce += sizeof(m_LinkState)-1;
	palce += sizeof(m_LinkType)-1;
	palce += sizeof(m_Bandwidth)-1;
	palce += sizeof(m_BandwidthUtilization)-1;
	palce += sizeof(m_SrcNodeId)-1;
	palce += sizeof(m_StartInterfaceIp)-1;
	palce += sizeof(m_SrcNodeInterface)-1;
	palce += sizeof(m_StartStatus)-1;
	palce += sizeof(m_StartErrorRate)-1;
	palce += sizeof(m_StartJitter)-1;
	palce += sizeof(m_StartAvgrtt)-1;
	palce += sizeof(m_StartPacketloss)-1;
	palce += sizeof(m_DstNodeId)-1;
	palce += sizeof(m_EndInterfaceIp)-1;
	palce += sizeof(m_DstNodeInterface)-1;
	palce += sizeof(m_EndStatus)-1;
	palce += sizeof(m_EndErrorRate)-1;
	palce += sizeof(m_EndJitter)-1;
	palce += sizeof(m_EndAvgrtt)-1;
	palce += sizeof(m_EndPacketloss)-1;

	return palce;
}

//���ķַ�����
int CNetworkInformation::MaxHandoutNum()
{
	//1024�ֽڿ��ԷŶ��ٸ����ݰ���
	int nMaxNum = floor(float(1024/FillDataSize()));

	//����������ݿ��Էֳɼ�����
	return ceil((float)m_listLines.size()/(float)nMaxNum);
}

//���ݰ�ͷ
bool CNetworkInformation::FillHeader(char *buffer, int &bufferLen)
{
	//��װ�ṹ��
	int palce = 0;

	//֡ͷ��־HEAD
	memcpy(buffer+palce, m_HEAD, sizeof(m_HEAD)-1);
	palce += sizeof(m_HEAD)-1;

	//���ޱ�ʶDA
	memcpy(buffer+palce, m_SA, sizeof(m_SA)-1);
	palce += sizeof(m_SA)-1;

	//��Դ��ʶSA
	memcpy(buffer+palce, m_DA, sizeof(m_DA)-1);
	palce += sizeof(m_DA)-1;

	//֡���� LEN
	m_nSignPlace = palce;
	memcpy(buffer+palce, m_LEN, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN)-1;

	//֡�������� FT
	YCodec::HexString2Bytes("0x83", m_FT, sizeof(m_FT)-1);
	memcpy(buffer+palce, m_FT, sizeof(m_FT)-1);
	palce += sizeof(m_FT)-1;

	//֡��ˮ�� SEQ
	if (!YCodec::IntToBytes(m_nFlowNumber, m_SEQ, sizeof(m_SEQ)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_SEQ, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ)-1;

	bufferLen = palce;
	
	return true;
}

//���ݲ���
bool CNetworkInformation::FillHandle(char *buffer,int &bufferLen)
{
	int palce = 0;
	//ȷ�ϱ�ʶ
	if (!YCodec::IntToBytes(1, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1);
	palce += sizeof(m_ConfirmLogo)-1;

	//Ӧ�������ʶ
	memcpy(buffer+palce, m_MIMA, sizeof(m_MIMA)-1);
	palce += sizeof(m_MIMA)-1;

	//���������ܰ���
	if (!YCodec::IntToBytes(MaxHandoutNum(), m_TaskToTal, sizeof(m_TaskToTal)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_TaskToTal, sizeof(m_TaskToTal)-1);
	palce += sizeof(m_TaskToTal)-1;

	//����Ӧ������
	memcpy(buffer+palce, m_QUPA, sizeof(m_QUPA)-1);
	palce += sizeof(m_QUPA)-1;

	//�����ϱ���·����
	if (!YCodec::IntToBytes(m_listLines.size(), m_LinkNumber, sizeof(m_LinkNumber)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_LinkNumber, sizeof(m_LinkNumber)-1);
	palce += sizeof(m_LinkNumber)-1;

	bufferLen = palce;

	return true;
}

//��ȡ��������
bool CNetworkInformation::FillData(NETELEMENTLINE elementline,char *buffer,int &bufferLen)
{
	int palce = 0;
	memset(buffer, 0, sizeof(buffer));

	//��·״̬
	if (!AddLinkState(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_LinkState, sizeof(m_LinkState)-1);
	palce += sizeof(m_LinkState)-1;

	//��·����
	if (!AddLinkType(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_LinkType, sizeof(m_LinkType)-1);
	palce += sizeof(m_LinkType)-1;

	//�ŵ�����
	if (!AddBandwidth(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_Bandwidth, sizeof(m_Bandwidth)-1);
	palce += sizeof(m_Bandwidth)-1;

	//ҵ�����ռ��
	if (!AddBandwidthUtilization(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_BandwidthUtilization, sizeof(m_BandwidthUtilization)-1);
	palce += sizeof(m_BandwidthUtilization)-1;

	//��������ID
	if (!AddSrcNodeId(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_SrcNodeId, sizeof(m_SrcNodeId)-1);
	palce += sizeof(m_SrcNodeId)-1;

	//��������IP
	if (!AddStartInterfaceIp(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartInterfaceIp, sizeof(m_StartInterfaceIp)-1);
	palce += sizeof(m_StartInterfaceIp)-1;

	//���������˿�
	if (!AddSrcNodeInterface(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_SrcNodeInterface, sizeof(m_SrcNodeInterface)-1);
	palce += sizeof(m_SrcNodeInterface)-1;

	//��������״̬
	if (!AddStartStatus(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartStatus, sizeof(m_StartStatus)-1);
	palce += sizeof(m_StartStatus)-1;

	//������������
	if (!AddStartErrorRate(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartErrorRate, sizeof(m_StartErrorRate)-1);
	palce += sizeof(m_StartErrorRate)-1;

	//��㴫��ʱ��
	if (!AddStartJitter(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartJitter, sizeof(m_StartJitter)-1);
	palce += sizeof(m_StartJitter)-1;

	//��㴫�䶶��
	if (!AddStartAvgrtt(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartAvgrtt, sizeof(m_StartAvgrtt)-1);
	palce += sizeof(m_StartAvgrtt)-1;

	//��㶪����
	if (!AddStartPacketloss(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartPacketloss, sizeof(m_StartPacketloss)-1);
	palce += sizeof(m_StartPacketloss)-1;

	//�յ������ID
	if (!AddDstNodeId(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_DstNodeId, sizeof(m_DstNodeId)-1);
	palce += sizeof(m_DstNodeId)-1;

	//�յ������IP
	if (!AddEndInterfaceIp(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndInterfaceIp, sizeof(m_EndInterfaceIp)-1);
	palce += sizeof(m_EndInterfaceIp)-1;

	//�յ�������˿�
	if (!AddDstNodeInterface(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_DstNodeInterface, sizeof(m_DstNodeInterface)-1);
	palce += sizeof(m_DstNodeInterface)-1;

	//�յ������״̬
	if (!AddEndStatus(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndStatus, sizeof(m_EndStatus)-1);
	palce += sizeof(m_EndStatus)-1;

	//�յ����������
	if (!AddEndErrorRate(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndErrorRate, sizeof(m_EndErrorRate)-1);
	palce += sizeof(m_EndErrorRate)-1;

	//�յ㴫��ʱ��
	if (!AddEndJitter(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndJitter, sizeof(m_EndJitter)-1);
	palce += sizeof(m_EndJitter)-1;

	//�յ㴫�䶶��
	if (!AddEndAvgrtt(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndAvgrtt, sizeof(m_EndAvgrtt)-1);
	palce += sizeof(m_EndAvgrtt)-1;

	//�յ㶪���� 
	if (!AddEndPacketloss(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndPacketloss, sizeof(m_EndPacketloss)-1);
	palce += sizeof(m_EndPacketloss)-1;
	bufferLen = palce;

	return true;
}

//���ݰ���֤
bool CNetworkInformation::FillVerify(int &bufferLen)
{
	//����д֡���� LEN
	memset(m_LEN, 0, sizeof(m_LEN));
	if (!YCodec::IntToBytes(bufferLen,m_LEN,sizeof(m_LEN)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+m_nSignPlace, m_LEN, sizeof(m_LEN)-1);

	//֡У�� FCS
	short int nfce = YCodec::crc_ccitt(m_szResponsePackage+(sizeof(m_HEAD)-1)+(sizeof(m_DA)-1), bufferLen-(sizeof(m_HEAD)-1)-(sizeof(m_DA)-1));
	if (!YCodec::IntToBytes(nfce,m_FCS,sizeof(m_FCS)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+bufferLen, m_FCS, sizeof(m_FCS)-1);
	bufferLen += sizeof(m_FCS)-1;
	return true;
}

//��·״̬
bool CNetworkInformation::AddLinkState(NETELEMENTLINE line)
{
	memset(m_LinkState, 0 ,sizeof(m_LinkState));
	if (!YCodec::IntToBytes(line.m_nStatus, m_LinkState, sizeof(m_LinkState)-1))
	{
		return false;
	}
	return true;
}

//��·����
bool CNetworkInformation::AddLinkType(NETELEMENTLINE line)
{
	memset(m_LinkType, 0, sizeof(m_LinkType));
	switch (line.m_oInterfaces.m_nChannelType)
	{
	case 1:
		charIntToByte(3, m_LinkType);
		break;
	case 2:
		charIntToByte(4, m_LinkType);
		break;
	case 4:
		charIntToByte(2, m_LinkType);
		break;
	case 5:
		charIntToByte(1, m_LinkType);
		break;
	}
	return true;
}

//�ŵ�����
bool CNetworkInformation::AddBandwidth(NETELEMENTLINE line)
{
	memset(m_Bandwidth, 0, sizeof(m_Bandwidth));
	int nBandwidth = line.m_oInterfaces.m_nBandwidth;

	if (nBandwidth <= 100)
	{
		YCodec::HexString2Bytes("0x21", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (100 < nBandwidth && nBandwidth <= 600)
	{
		YCodec::HexString2Bytes("0x22", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (600 < nBandwidth && nBandwidth <= 1200)
	{
		YCodec::HexString2Bytes("0x23", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (1200 < nBandwidth && nBandwidth <= 2400)
	{
		YCodec::HexString2Bytes("0x24", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (2400 < nBandwidth && nBandwidth <= 9600)
	{
		YCodec::HexString2Bytes("0x25", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (9600 < nBandwidth && nBandwidth <= 2000000)
	{
		YCodec::HexString2Bytes("0x41", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (2000000 < nBandwidth && nBandwidth <= 4000000)
	{
		YCodec::HexString2Bytes("0x44", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (4000000 < nBandwidth && nBandwidth <= 8000000)
	{
		YCodec::HexString2Bytes("0x48", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	else if (8000000 < nBandwidth)
	{
		YCodec::HexString2Bytes("0x48", m_Bandwidth, sizeof(m_Bandwidth)-1);
	}
	
	return true;
}

//ҵ�����ռ��
bool CNetworkInformation::AddBandwidthUtilization(NETELEMENTLINE line)
{
	memset(m_BandwidthUtilization, 0, sizeof(m_BandwidthUtilization));
	//charIntToByte(ceil(line.m_oInterfaces.m_fBandwidthUtilization * 100),m_BandwidthUtilization);
	if (line.m_oInterfaces.m_fBandwidthUtilization < 0.0)
	{
		if (!YCodec::IntToBytes(0, m_BandwidthUtilization, sizeof(m_BandwidthUtilization)-1))
		{
			return false;
		}
	}
	else if(line.m_oInterfaces.m_fBandwidthUtilization > 1.0)
	{
		if (!YCodec::IntToBytes(100, m_BandwidthUtilization, sizeof(m_BandwidthUtilization)-1))
		{
			return false;
		}
	}
	else
	{
		if (!YCodec::IntToBytes(ceil(line.m_oInterfaces.m_fBandwidthUtilization * 100), m_BandwidthUtilization, sizeof(m_BandwidthUtilization)-1))
		{
			return false;
		}
	}
	
	return true;
}

//��������ID
bool CNetworkInformation::AddSrcNodeId(NETELEMENTLINE line)
{
	memset(m_SrcNodeId, 0, sizeof(m_SrcNodeId));; 
	if (!YCodec::IntToBytes(line.m_nSrcDEVId, m_SrcNodeId, sizeof(m_SrcNodeId)-1))
	{
		return false;
	}
	return true;
}

//��������IP
bool CNetworkInformation::AddStartInterfaceIp(NETELEMENTLINE line)
{
	memset(m_StartInterfaceIp,0,sizeof(m_StartInterfaceIp));
	unsigned int nInterfaceIp = Tools::string_to_unsigned_int(line.m_SrcPoint.m_strInterfaceIp);

	if (!YCodec::IntToBytes_Network(nInterfaceIp,m_StartInterfaceIp,sizeof(m_StartInterfaceIp)-1))
	{
		return false;
	}
    return true;
}

//���������˿�
bool CNetworkInformation::AddSrcNodeInterface(NETELEMENTLINE line)
{
	memset(m_SrcNodeInterface,0,sizeof(m_SrcNodeInterface));
	charIntToByte(line.m_nSrcIndex,m_SrcNodeInterface);
	return true;
}

//��������״̬
bool CNetworkInformation::AddStartStatus(NETELEMENTLINE line)
{
	memset(m_StartStatus,0,sizeof(m_StartStatus));
	charIntToByte(line.m_SrcPoint.m_nstatus, m_StartStatus);
	return true;
}

//������������
bool CNetworkInformation::AddStartErrorRate(NETELEMENTLINE line)
{
	char strErrorrate[30];
	memset(m_StartErrorRate,0,sizeof(m_StartErrorRate));
	memset(strErrorrate,0,sizeof(strErrorrate));
	YCodec::Double2str(line.m_SrcPoint.m_fErrorRate,strErrorrate);
	YCodec::HexString2Bytes(strErrorrate, m_StartErrorRate, sizeof(m_StartErrorRate)-1);

	return true;
}

//��㴫��ʱ��
bool CNetworkInformation::AddStartJitter(NETELEMENTLINE line)
{
	memset(m_StartJitter,0,sizeof(m_StartJitter));
	if (!YCodec::IntToBytes(ceil(line.m_SrcPoint.m_favgrtt),m_StartJitter,sizeof(m_StartJitter)-1))
	{
		return false;
	}
	return true;
}

//��㴫�䶶��
bool CNetworkInformation::AddStartAvgrtt(NETELEMENTLINE line)
{
	memset(m_StartAvgrtt,0,sizeof(m_StartAvgrtt));

	if (line.m_SrcPoint.m_njitter >= 1000)
	{
		if (!YCodec::IntToBytes(ceil((float)(line.m_SrcPoint.m_njitter/1000)),m_StartAvgrtt,sizeof(m_StartAvgrtt)-1))
		{
			return false;
		}
	}
	else if (line.m_SrcPoint.m_njitter < 1000)
	{
		if (!YCodec::IntToBytes(1,m_StartAvgrtt,sizeof(m_StartAvgrtt)-1))
		{
			return false;
		}
	}
	
	return true;
}

//��㶪����
bool CNetworkInformation::AddStartPacketloss(NETELEMENTLINE line)
{
	memset(m_StartPacketloss,0,sizeof(m_StartPacketloss));
	if (line.m_SrcPoint.m_fpacketloss > 100.0)
	{
		if (!YCodec::IntToBytes(100,m_StartPacketloss,sizeof(m_StartPacketloss)-1))
		{
			return false;
		}
	}
	else
	{
		if (!YCodec::IntToBytes(ceil(line.m_SrcPoint.m_fpacketloss),m_StartPacketloss,sizeof(m_StartPacketloss)-1))
		{
			return false;
		}
	}
	return true;
}

//�յ������ID
bool CNetworkInformation::AddDstNodeId(NETELEMENTLINE line)
{
	memset(m_DstNodeId,0,sizeof(m_DstNodeId));
	if (!YCodec::IntToBytes(line.m_nDstDEVId,m_DstNodeId,sizeof(m_DstNodeId)-1))
	{
		return false;
	}
	return true;
}

//�յ������IP
bool CNetworkInformation::AddEndInterfaceIp(NETELEMENTLINE line)
{
	memset(m_EndInterfaceIp,0,sizeof(m_EndInterfaceIp));
	unsigned int nInterfaceIp = Tools::string_to_unsigned_int(line.m_DstPoint.m_strInterfaceIp);

	if (!YCodec::IntToBytes_Network(nInterfaceIp,m_EndInterfaceIp,sizeof(m_EndInterfaceIp)-1))
	{
		return false;
	}
	return true;
}

//�յ�������˿�
bool CNetworkInformation::AddDstNodeInterface(NETELEMENTLINE line)
{
	memset(m_DstNodeInterface,0,sizeof(m_DstNodeInterface));
	charIntToByte(line.m_nDstIndex,m_DstNodeInterface);
	return true;
}

//�յ������״̬
bool CNetworkInformation::AddEndStatus(NETELEMENTLINE line)
{
	memset(m_EndStatus,0,sizeof(m_EndStatus));
	charIntToByte(line.m_DstPoint.m_nstatus, m_EndStatus);
	return true;
}

//�յ����������
bool CNetworkInformation::AddEndErrorRate(NETELEMENTLINE line)
{
	char strResult[30];
	memset(strResult,0,sizeof(strResult));
	memset(m_EndErrorRate,0,sizeof(m_EndErrorRate));
	YCodec::Double2str(line.m_DstPoint.m_fErrorRate,strResult);
	YCodec::HexString2Bytes(strResult, m_EndErrorRate, sizeof(m_EndErrorRate)-1);

	return true;
}

//�յ㴫��ʱ��
bool CNetworkInformation::AddEndJitter(NETELEMENTLINE line)
{
	memset(m_EndJitter,0,sizeof(m_EndJitter));
	if (!YCodec::IntToBytes(ceil(line.m_DstPoint.m_favgrtt),m_EndJitter,sizeof(m_EndJitter)-1))
	{
		return false;
	}
	return true;
}

//�յ㴫�䶶��
bool CNetworkInformation::AddEndAvgrtt(NETELEMENTLINE line)
{
	memset(m_EndAvgrtt,0,sizeof(m_EndAvgrtt));
	if (line.m_DstPoint.m_njitter >= 1000)
	{
		if (!YCodec::IntToBytes(ceil((float)(line.m_DstPoint.m_njitter/1000)),m_EndAvgrtt,sizeof(m_EndAvgrtt)-1))
		{
			return false;
		}
	}
	else if (line.m_DstPoint.m_njitter < 1000)
	{
		if (!YCodec::IntToBytes(1,m_EndAvgrtt,sizeof(m_EndAvgrtt)-1))
		{
			return false;
		}
	}
	
	return true;
}

 //�յ㶪����
bool CNetworkInformation::AddEndPacketloss(NETELEMENTLINE line)
{
	memset(m_EndPacketloss,0,sizeof(m_EndPacketloss));
	if (line.m_DstPoint.m_fpacketloss > 100.0)
	{
		if (!YCodec::IntToBytes(100,m_EndPacketloss,sizeof(m_EndPacketloss)-1))
		{
			return false;
		}
	}
	else
	{
		if (!YCodec::IntToBytes(ceil(line.m_DstPoint.m_fpacketloss),m_EndPacketloss,sizeof(m_EndPacketloss)-1))
		{
			return false;
		}
	}
	return true;
}

void CNetworkInformation::charIntToByte(int i, char *bytes)
{
	//byte[] bytes = new byte[2];
	memset(bytes, 0, sizeof(char)*2);
	bytes[0] = (char) (0xff & i);
}
