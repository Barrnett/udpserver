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

	//查询任务标识
	memcpy(m_MIMA, data+palce, sizeof(m_MIMA)-1);
	palce += sizeof(m_MIMA) -1;

	//最大数据包长度
	memcpy(m_MAPA, data+palce, sizeof(m_MAPA)-1);
	palce += sizeof(m_MAPA) -1;
	
	//本次查询包序号
	memcpy(m_QUPA, data+palce, sizeof(m_QUPA)-1);
	palce += sizeof(m_QUPA) -1;

	//帧校验 FCS
	memcpy(m_FCS, data+palce, sizeof(m_FCS)-1);

	return true;
}

//回复包
bool CNetworkInformation::Response(int flowNumber)
{
	Clear();
	ClearResponse();

	int packetOrder = 0;
	if (!YCodec::bytesToInt(m_QUPA, packetOrder, sizeof(m_QUPA)-1))
	{
		//提取包序失败
		return false;
	}

	//重新取数据
	if (packetOrder == 1)     //重新获取包
	{
		if (!GetDataBase())
		{
			//提取数据失败
			return false;
		}
		//正常应答
		return ResponseNormal(packetOrder);
	}
	else if (packetOrder == m_nPacketOrder + 1)
	{
		//正常应答
		return ResponseNormal(packetOrder);
	}
	else
	{
		//否认应答
		return ResponseDeny(packetOrder);
	}
	return false;
}

//正常应答
bool CNetworkInformation::ResponseNormal(int packetOrder)
{
	ClearResponse();

	int bufferLen = 0;
	int bufferTotalLen = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));
	//组装包头
	if (!FillHeader(m_szResponsePackage, bufferLen))
	{
		return false;
	}
	bufferTotalLen = bufferLen;
	//组装包操作
	if (!FillHandle(m_szResponsePackage+bufferTotalLen, bufferLen))
	{
		return false;
	}
	bufferTotalLen += bufferLen;

	//1024字节可以放多少个数据包体
	int nMaxNum = floor(float(1024/FillDataSize()));
	//容器开始下标
	int nIndex = (packetOrder-1)*nMaxNum;


	//组装包体数据
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

	//组装验证
	if (!FillVerify(bufferTotalLen))
	{
		return false;
	}

	m_nResponsePackageLen = bufferTotalLen;
	return true;
}


//否认应答
bool CNetworkInformation::ResponseDeny(int packetOrder)
{
	ClearResponse();

	//组装结构包
	int nPalce = 0;
	int nSignPlace = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));


	//帧头标志HEAD
	memcpy(m_szResponsePackage, m_HEAD, sizeof(m_HEAD)-1);
	nPalce += sizeof(m_HEAD)-1;

	//信宿标识DA
	memcpy(m_szResponsePackage+nPalce, m_SA, sizeof(m_SA)-1);
	nPalce += sizeof(m_SA)-1;

	//信源标识SA
	memcpy(m_szResponsePackage+nPalce, m_DA, sizeof(m_DA)-1);
	nPalce += sizeof(m_DA)-1;
	nSignPlace = nPalce;

	//帧长度 LEN
	memcpy(m_szResponsePackage+nPalce, m_LEN, sizeof(m_LEN)-1);
	nPalce += sizeof(m_LEN)-1;

	//帧命令类型 FT
	YCodec::HexString2Bytes("83", m_FT, sizeof(m_FT)-1);
	memcpy(m_szResponsePackage+nPalce, m_FT, sizeof(m_FT)-1);
	nPalce += sizeof(m_FT)-1;

	//帧流水号 SEQ
	YCodec::intToBytes(m_nFlowNumber, m_SEQ);
	memcpy(m_szResponsePackage+nPalce, m_SEQ, sizeof(m_SEQ)-1);
	nPalce += sizeof(m_SEQ)-1;

	//确认标识
	if (!YCodec::IntToBytes(3, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nPalce, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1);
	nPalce += sizeof(m_ConfirmLogo)-1;

	//查询任务标识
	memcpy(m_szResponsePackage+nPalce, m_MIMA, sizeof(m_MIMA)-1);
	nPalce += sizeof(m_ReplyLogo)-1;

	//期望的包序号
	if (!YCodec::IntToBytes(packetOrder, m_ExpePack))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nPalce, m_ExpePack, sizeof(m_ExpePack)-1);
	nPalce += sizeof(m_ExpePack)-1;

	//重新写帧长度 LEN
	if (!YCodec::IntToBytes(nPalce,m_LEN,sizeof(m_LEN)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+nSignPlace, m_LEN, sizeof(m_LEN)-1);

	//帧校验 FCS
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


//获取数据库数据
bool CNetworkInformation::GetDataBase()
{
	//删除容器数据
	m_listLines.clear();
	/*
	list<NETELEMENTLINE>::iterator iter;
	for (iter = m_listLines.begin(); iter != m_listLines.end(); iter++)
	{
		iter = m_listLines.erase(iter);
	}
	*/

	//获取容器数据
	string strError;
	if (!CDb::getLines(m_listLines, strError))
	{
		return false;
	}
	return true;
}


//填充数据包大小
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

//最大的分发包数
int CNetworkInformation::MaxHandoutNum()
{
	//1024字节可以放多少个数据包体
	int nMaxNum = floor(float(1024/FillDataSize()));

	//容器里的数据可以分成几个包
	return ceil((float)m_listLines.size()/(float)nMaxNum);
}

//数据包头
bool CNetworkInformation::FillHeader(char *buffer, int &bufferLen)
{
	//组装结构包
	int palce = 0;

	//帧头标志HEAD
	memcpy(buffer+palce, m_HEAD, sizeof(m_HEAD)-1);
	palce += sizeof(m_HEAD)-1;

	//信宿标识DA
	memcpy(buffer+palce, m_SA, sizeof(m_SA)-1);
	palce += sizeof(m_SA)-1;

	//信源标识SA
	memcpy(buffer+palce, m_DA, sizeof(m_DA)-1);
	palce += sizeof(m_DA)-1;

	//帧长度 LEN
	m_nSignPlace = palce;
	memcpy(buffer+palce, m_LEN, sizeof(m_LEN)-1);
	palce += sizeof(m_LEN)-1;

	//帧命令类型 FT
	YCodec::HexString2Bytes("0x83", m_FT, sizeof(m_FT)-1);
	memcpy(buffer+palce, m_FT, sizeof(m_FT)-1);
	palce += sizeof(m_FT)-1;

	//帧流水号 SEQ
	if (!YCodec::IntToBytes(m_nFlowNumber, m_SEQ, sizeof(m_SEQ)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_SEQ, sizeof(m_SEQ)-1);
	palce += sizeof(m_SEQ)-1;

	bufferLen = palce;
	
	return true;
}

//数据操作
bool CNetworkInformation::FillHandle(char *buffer,int &bufferLen)
{
	int palce = 0;
	//确认标识
	if (!YCodec::IntToBytes(1, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_ConfirmLogo, sizeof(m_ConfirmLogo)-1);
	palce += sizeof(m_ConfirmLogo)-1;

	//应答任务标识
	memcpy(buffer+palce, m_MIMA, sizeof(m_MIMA)-1);
	palce += sizeof(m_MIMA)-1;

	//任务数据总包数
	if (!YCodec::IntToBytes(MaxHandoutNum(), m_TaskToTal, sizeof(m_TaskToTal)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_TaskToTal, sizeof(m_TaskToTal)-1);
	palce += sizeof(m_TaskToTal)-1;

	//本次应答包序号
	memcpy(buffer+palce, m_QUPA, sizeof(m_QUPA)-1);
	palce += sizeof(m_QUPA)-1;

	//本次上报链路数量
	if (!YCodec::IntToBytes(m_listLines.size(), m_LinkNumber, sizeof(m_LinkNumber)-1))
	{
		return false;
	}
	memcpy(buffer+palce, m_LinkNumber, sizeof(m_LinkNumber)-1);
	palce += sizeof(m_LinkNumber)-1;

	bufferLen = palce;

	return true;
}

//获取包体数据
bool CNetworkInformation::FillData(NETELEMENTLINE elementline,char *buffer,int &bufferLen)
{
	int palce = 0;
	memset(buffer, 0, sizeof(buffer));

	//链路状态
	if (!AddLinkState(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_LinkState, sizeof(m_LinkState)-1);
	palce += sizeof(m_LinkState)-1;

	//链路类型
	if (!AddLinkType(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_LinkType, sizeof(m_LinkType)-1);
	palce += sizeof(m_LinkType)-1;

	//信道带宽
	if (!AddBandwidth(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_Bandwidth, sizeof(m_Bandwidth)-1);
	palce += sizeof(m_Bandwidth)-1;

	//业务带宽占比
	if (!AddBandwidthUtilization(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_BandwidthUtilization, sizeof(m_BandwidthUtilization)-1);
	palce += sizeof(m_BandwidthUtilization)-1;

	//起点控制器ID
	if (!AddSrcNodeId(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_SrcNodeId, sizeof(m_SrcNodeId)-1);
	palce += sizeof(m_SrcNodeId)-1;

	//起点控制器IP
	if (!AddStartInterfaceIp(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartInterfaceIp, sizeof(m_StartInterfaceIp)-1);
	palce += sizeof(m_StartInterfaceIp)-1;

	//起点控制器端口
	if (!AddSrcNodeInterface(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_SrcNodeInterface, sizeof(m_SrcNodeInterface)-1);
	palce += sizeof(m_SrcNodeInterface)-1;

	//起点控制器状态
	if (!AddStartStatus(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartStatus, sizeof(m_StartStatus)-1);
	palce += sizeof(m_StartStatus)-1;

	//起点接收误码率
	if (!AddStartErrorRate(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartErrorRate, sizeof(m_StartErrorRate)-1);
	palce += sizeof(m_StartErrorRate)-1;

	//起点传输时延
	if (!AddStartJitter(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartJitter, sizeof(m_StartJitter)-1);
	palce += sizeof(m_StartJitter)-1;

	//起点传输抖动
	if (!AddStartAvgrtt(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartAvgrtt, sizeof(m_StartAvgrtt)-1);
	palce += sizeof(m_StartAvgrtt)-1;

	//起点丢包率
	if (!AddStartPacketloss(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_StartPacketloss, sizeof(m_StartPacketloss)-1);
	palce += sizeof(m_StartPacketloss)-1;

	//终点控制器ID
	if (!AddDstNodeId(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_DstNodeId, sizeof(m_DstNodeId)-1);
	palce += sizeof(m_DstNodeId)-1;

	//终点控制器IP
	if (!AddEndInterfaceIp(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndInterfaceIp, sizeof(m_EndInterfaceIp)-1);
	palce += sizeof(m_EndInterfaceIp)-1;

	//终点控制器端口
	if (!AddDstNodeInterface(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_DstNodeInterface, sizeof(m_DstNodeInterface)-1);
	palce += sizeof(m_DstNodeInterface)-1;

	//终点控制器状态
	if (!AddEndStatus(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndStatus, sizeof(m_EndStatus)-1);
	palce += sizeof(m_EndStatus)-1;

	//终点接收误码率
	if (!AddEndErrorRate(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndErrorRate, sizeof(m_EndErrorRate)-1);
	palce += sizeof(m_EndErrorRate)-1;

	//终点传输时延
	if (!AddEndJitter(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndJitter, sizeof(m_EndJitter)-1);
	palce += sizeof(m_EndJitter)-1;

	//终点传输抖动
	if (!AddEndAvgrtt(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndAvgrtt, sizeof(m_EndAvgrtt)-1);
	palce += sizeof(m_EndAvgrtt)-1;

	//终点丢包率 
	if (!AddEndPacketloss(elementline))
	{
		return false;
	}
	memcpy(buffer+palce, m_EndPacketloss, sizeof(m_EndPacketloss)-1);
	palce += sizeof(m_EndPacketloss)-1;
	bufferLen = palce;

	return true;
}

//数据包验证
bool CNetworkInformation::FillVerify(int &bufferLen)
{
	//重新写帧长度 LEN
	memset(m_LEN, 0, sizeof(m_LEN));
	if (!YCodec::IntToBytes(bufferLen,m_LEN,sizeof(m_LEN)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+m_nSignPlace, m_LEN, sizeof(m_LEN)-1);

	//帧校验 FCS
	short int nfce = YCodec::crc_ccitt(m_szResponsePackage+(sizeof(m_HEAD)-1)+(sizeof(m_DA)-1), bufferLen-(sizeof(m_HEAD)-1)-(sizeof(m_DA)-1));
	if (!YCodec::IntToBytes(nfce,m_FCS,sizeof(m_FCS)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+bufferLen, m_FCS, sizeof(m_FCS)-1);
	bufferLen += sizeof(m_FCS)-1;
	return true;
}

//链路状态
bool CNetworkInformation::AddLinkState(NETELEMENTLINE line)
{
	memset(m_LinkState, 0 ,sizeof(m_LinkState));
	if (!YCodec::IntToBytes(line.m_nStatus, m_LinkState, sizeof(m_LinkState)-1))
	{
		return false;
	}
	return true;
}

//链路类型
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

//信道带宽
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

//业务带宽占比
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

//起点控制器ID
bool CNetworkInformation::AddSrcNodeId(NETELEMENTLINE line)
{
	memset(m_SrcNodeId, 0, sizeof(m_SrcNodeId));; 
	if (!YCodec::IntToBytes(line.m_nSrcDEVId, m_SrcNodeId, sizeof(m_SrcNodeId)-1))
	{
		return false;
	}
	return true;
}

//起点控制器IP
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

//起点控制器端口
bool CNetworkInformation::AddSrcNodeInterface(NETELEMENTLINE line)
{
	memset(m_SrcNodeInterface,0,sizeof(m_SrcNodeInterface));
	charIntToByte(line.m_nSrcIndex,m_SrcNodeInterface);
	return true;
}

//起点控制器状态
bool CNetworkInformation::AddStartStatus(NETELEMENTLINE line)
{
	memset(m_StartStatus,0,sizeof(m_StartStatus));
	charIntToByte(line.m_SrcPoint.m_nstatus, m_StartStatus);
	return true;
}

//起点接收误码率
bool CNetworkInformation::AddStartErrorRate(NETELEMENTLINE line)
{
	char strErrorrate[30];
	memset(m_StartErrorRate,0,sizeof(m_StartErrorRate));
	memset(strErrorrate,0,sizeof(strErrorrate));
	YCodec::Double2str(line.m_SrcPoint.m_fErrorRate,strErrorrate);
	YCodec::HexString2Bytes(strErrorrate, m_StartErrorRate, sizeof(m_StartErrorRate)-1);

	return true;
}

//起点传输时延
bool CNetworkInformation::AddStartJitter(NETELEMENTLINE line)
{
	memset(m_StartJitter,0,sizeof(m_StartJitter));
	if (!YCodec::IntToBytes(ceil(line.m_SrcPoint.m_favgrtt),m_StartJitter,sizeof(m_StartJitter)-1))
	{
		return false;
	}
	return true;
}

//起点传输抖动
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

//起点丢包率
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

//终点控制器ID
bool CNetworkInformation::AddDstNodeId(NETELEMENTLINE line)
{
	memset(m_DstNodeId,0,sizeof(m_DstNodeId));
	if (!YCodec::IntToBytes(line.m_nDstDEVId,m_DstNodeId,sizeof(m_DstNodeId)-1))
	{
		return false;
	}
	return true;
}

//终点控制器IP
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

//终点控制器端口
bool CNetworkInformation::AddDstNodeInterface(NETELEMENTLINE line)
{
	memset(m_DstNodeInterface,0,sizeof(m_DstNodeInterface));
	charIntToByte(line.m_nDstIndex,m_DstNodeInterface);
	return true;
}

//终点控制器状态
bool CNetworkInformation::AddEndStatus(NETELEMENTLINE line)
{
	memset(m_EndStatus,0,sizeof(m_EndStatus));
	charIntToByte(line.m_DstPoint.m_nstatus, m_EndStatus);
	return true;
}

//终点接收误码率
bool CNetworkInformation::AddEndErrorRate(NETELEMENTLINE line)
{
	char strResult[30];
	memset(strResult,0,sizeof(strResult));
	memset(m_EndErrorRate,0,sizeof(m_EndErrorRate));
	YCodec::Double2str(line.m_DstPoint.m_fErrorRate,strResult);
	YCodec::HexString2Bytes(strResult, m_EndErrorRate, sizeof(m_EndErrorRate)-1);

	return true;
}

//终点传输时延
bool CNetworkInformation::AddEndJitter(NETELEMENTLINE line)
{
	memset(m_EndJitter,0,sizeof(m_EndJitter));
	if (!YCodec::IntToBytes(ceil(line.m_DstPoint.m_favgrtt),m_EndJitter,sizeof(m_EndJitter)-1))
	{
		return false;
	}
	return true;
}

//终点传输抖动
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

 //终点丢包率
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
