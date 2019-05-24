#include "OptimalPathInformation.h"
#include <assert.h>
#include <memory.h>
#include <string.h>
#include "MxHandlePool.h"
#include "../MXDatabase/NetElementDb.h"
#include "../Tool/YCodec.h"


COptimalPathInformation::COptimalPathInformation()
{
	Clear();
}

COptimalPathInformation::~COptimalPathInformation()
{
	Clear();
}

void COptimalPathInformation::Clear()
{
	m_nResponsePackageLen = 0;
	memset(m_szResponsePackage, 0, sizeof(m_szResponsePackage));

	memset(m_HEAD, 0, sizeof(m_HEAD));
	memset(m_DA, 0, sizeof(m_DA));
	memset(m_SA, 0, sizeof(m_SA));
	memset(m_LEN, 0, sizeof(m_LEN));
	memset(m_FT, 0, sizeof(m_FT));
	memset(m_SEQ, 0, sizeof(m_SEQ));
	memset(m_TALO, 0, sizeof(m_TALO));
	memset(m_SCO, 0, sizeof(m_SCO));
	memset(m_PCO, 0, sizeof(m_PCO));
	memset(m_FCS, 0, sizeof(m_FCS));

	//
	memset(m_PathNums, 0, sizeof(m_PathNums));
	memset(m_PathLineNums, 0, sizeof(m_PathLineNums));
	memset(m_NodeInterface, 0, sizeof(m_NodeInterface));
	memset(m_NextNodeId, 0, sizeof(m_NextNodeId));
	memset(m_NextNodeInterface, 0, sizeof(m_NextNodeInterface));
}

bool COptimalPathInformation::CheckCommand(char* data)
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

	//任务标识
	memcpy(m_TALO, data+palce, sizeof(m_TALO)-1);
	palce += sizeof(m_TALO) -1;

	//源控制器
	memcpy(m_SCO, data+palce, sizeof(m_SCO)-1);
	palce += sizeof(m_SCO) -1;

	//目的控制器
	memcpy(m_PCO, data+palce, sizeof(m_PCO)-1);
	palce += sizeof(m_PCO) -1;

	//帧校验 FCS
	memcpy(m_FCS, data+palce, sizeof(m_FCS)-1);

	return true;
}

bool COptimalPathInformation::Response(int flowNumber)
{
	//组装结构包
	int palce = 0;
	int checkPlace = 0;
	int signPlace = 0;
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
	YCodec::HexString2Bytes("0x84", m_FT, sizeof(m_FT)-1);
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

	//查询任务标识
	memcpy(m_szResponsePackage+palce, m_TALO, sizeof(m_TALO)-1);
	palce += sizeof(m_TALO)-1;

	//源控制器ID
	memset(m_SCO, 0, sizeof(m_SCO));
	if (!YCodec::IntToBytes(204, m_SCO, sizeof(m_SCO)-1))
	{
		return false;
	}

	//目的控制器ID
	memset(m_PCO, 0, sizeof(m_PCO));
	if (!YCodec::IntToBytes(201, m_PCO, sizeof(m_PCO)-1))
	{
		return false;
	}
	memcpy(m_szResponsePackage+palce, m_PCO, sizeof(m_PCO)-1);
	palce += sizeof(m_PCO)-1;


	//最优路径信息(从数据库中取)
	string strError;
	std::list<OPTIMALPATH> optimalPathinfo;
	if (!CDb::getOptimalPath(optimalPathinfo, strError))
	{
		return false;
	}

	//遍历所有的vpx
	int strLength = 0;
	list<OPTIMALPATH>::iterator it;
	for (it = optimalPathinfo.begin(); it != optimalPathinfo.end(); it++)
	{
		/*寻找本地数据
		if (!it->bIsSelf)
		{
			continue;
		}
		*/
		//路径数量M
		if (!AddPathNum(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_PathNums, sizeof(m_PathNums)-1);
		palce += sizeof(m_PathNums)-1;
		
		//路径链路数量
		if (!AddPathLinkNum(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_PathLineNums, sizeof(m_PathLineNums)-1);
		palce += sizeof(m_PathLineNums)-1;
	
		//控制器出端口
		if (!AddGecOutPort(*it))
		{
			return false;

		}
		memcpy(m_szResponsePackage+palce, m_NodeInterface, sizeof(m_NodeInterface)-1);
		palce += sizeof(m_NodeInterface)-1;
		
		//下一跳控制器ID
		if (!AddNextGecId(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_NextNodeId, sizeof(m_NextNodeId)-1);
		palce += sizeof(m_NextNodeId)-1;

		//下一跳控制器入端口
		if (!AddNextGecInPort(*it))
		{
			return false;
		}
		memcpy(m_szResponsePackage+palce, m_NextNodeInterface, sizeof(m_NextNodeInterface)-1);
		palce += sizeof(m_NextNodeInterface)-1;

		//清空缓存
		optimalPathinfo.clear();

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

//路径数量M
bool COptimalPathInformation::AddPathNum(OPTIMALPATH optimalPathinfo)
{
	memset(m_PathNums,0,sizeof(m_PathNums));
	if (!YCodec::IntToBytes(optimalPathinfo.m_pathNumber, m_PathNums, sizeof(m_PathNums)-1))
	{
		return false;
	}
	return true;

}

//路径链路数量N
bool COptimalPathInformation::AddPathLinkNum(OPTIMALPATH optimalPathinfo)
{
	memset(m_PathLineNums,0,sizeof(m_PathLineNums));
	if (!YCodec::IntToBytes(optimalPathinfo.m_pathLinkNumber, m_PathLineNums, sizeof(m_PathLineNums)-1))
	{
		return false;
	}
	return true;
}


//控制器出端口
bool COptimalPathInformation::AddGecOutPort(OPTIMALPATH optimalPathinfo)
{
	memset(m_NodeInterface,0,sizeof(m_NodeInterface));
	charIntToByte(optimalPathinfo.m_portOut,m_NodeInterface);
	return true;
}

//下一跳控制器ID
bool COptimalPathInformation::AddNextGecId(OPTIMALPATH optimalPathinfo)
{	
	memset(m_NextNodeId,0,sizeof(m_NextNodeId));
	if (!YCodec::IntToBytes(optimalPathinfo.m_nextGecoid, m_NextNodeId, sizeof(m_NextNodeId)-1))
	{
		return false;
	}
	return true;
}


//下一跳控制器入端口
bool COptimalPathInformation::AddNextGecInPort(OPTIMALPATH optimalPathinfo)
{
	memset(m_NextNodeInterface,0,sizeof(m_NextNodeInterface));
	if (!YCodec::IntToBytes(optimalPathinfo.m_nextGecoport, m_NextNodeInterface, sizeof(m_NextNodeInterface)-1))
	{
		return false;
	}
	return true;
}

	
void COptimalPathInformation::charIntToByte(int i, char *bytes)
{
	//byte[] bytes = new byte[2];
	memset(bytes, 0, sizeof(char)*2);
	
}


