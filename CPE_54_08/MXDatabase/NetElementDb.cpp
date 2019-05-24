#include "NetElementDb.h"
#include "../Work/fileRead.h"
extern NetConfig netConfig;

CDb::CDb(void)
{
}

CDb::~CDb(void)
{

}

//得到搜索的设备(1.1 - 1.2)
bool CDb::getSearchDevs(list<DEVICE> &lstDevs,string &strError)
{
	char szError[100] = {0};
	CMxHandleLoc mxHandleLoc;
	int nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopology","",ExistNodePath);
	if(nRet != BS_NOERROR)
	{
		sprintf(szError,"Topolopy open failed: %d",nRet);
		strError = szError;
		return false;
	}

	string selectMainKey = mxHandleLoc.Treedb_GetPropertyString("SelectMainKey","");

	if (!strcmp(selectMainKey.c_str(), "NetTopology"))
	{
		goto part2;
	}
	else if (!strcmp(selectMainKey.c_str(), "NetTopologyBack"))
	{
		nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopologyBack","",ExistNodePath);
		if(nRet != BS_NOERROR)
		{
			sprintf(szError,"Topolopy open failed: %d",nRet);
			strError = szError;
			return false;
		}
		goto part2;
	}

part2:

	unsigned int nSelfIp = 0;
	getAgentIp(nSelfIp,strError);

	nRet = mxHandleLoc.Treedb_ReopenSubKey("NetDevices");
	if(nRet != BS_NOERROR)
	{
		if(nRet == BS_ERROR_BPTREENOFIND)
		{
			// 当前没有线生成
			return true;
		}
		sprintf(szError,"NetDevices open failed:",nRet);
		strError = szError;
		return false;
	}

	char szLinePath[100] = {0};
	list<string> lstDevSn;
	nRet = mxHandleLoc.Treedb_GetAllSubKeys(lstDevSn);
	if(nRet != BS_NOERROR)
	{
		sprintf(szLinePath,"NetDevices get failed: %d",nRet);
		strError = szError;
		return false;
	}

	char szDevPath[500] = {0};
	list<string>::iterator iter_dev = lstDevSn.begin();
	for(;iter_dev != lstDevSn.end();iter_dev++)
	{
		string strDevSn = *iter_dev;
		sprintf(szDevPath,"%s\\%s","NetDevices",strDevSn.c_str());

		nRet = mxHandleLoc.Treedb_ReopenSubKey(szDevPath);
		if(nRet != BS_NOERROR)
		{
			continue;
		}

		int isActivity = mxHandleLoc.Treedb_GetPropertyInt("isActivity",0);
		if(isActivity == 1)
		{
			DEVICE device;
			device.m_strSN = *iter_dev;

			device.m_nId = mxHandleLoc.Treedb_GetPropertyInt("devid",-1);
			device.m_strName = mxHandleLoc.Treedb_GetPropertyString("DevName","");
			device.m_strNumber = mxHandleLoc.Treedb_GetPropertyString("DevNumber","");
			device.m_strSoftVersion = mxHandleLoc.Treedb_GetPropertyString("SoftVesion","");
			device.m_strBoardIp = mxHandleLoc.Treedb_GetPropertyString("MainboardIp","");
			device.m_strManageIp = mxHandleLoc.Treedb_GetPropertyString("JianKongIp","");
			device.m_nIsActivity = mxHandleLoc.Treedb_GetPropertyInt("isActivity",-1);
			device.m_nState = mxHandleLoc.Treedb_GetPropertyInt("status",-1);
			device.m_strLongittude = mxHandleLoc.Treedb_GetPropertyString("longitude","");
			device.m_strLatitude = mxHandleLoc.Treedb_GetPropertyString("latitude","");
			//就数据

			device.m_strDescribe = mxHandleLoc.Treedb_GetPropertyString("DevDescribe","");
			device.m_strSoftVersion = mxHandleLoc.Treedb_GetPropertyString("SoftVesion","");

			if(nSelfIp == atoi(device.m_strBoardIp.c_str()))
			{
				device.bIsSelf = true;
			}
			else
			{
				device.bIsSelf = false;
			}

			lstDevs.push_back(device);
		}
	}
	lstDevSn.clear();
	return true;
}

bool CDb::getAgentIp(unsigned int &nIp,string &strError)
{
	string strSubKeyPath = "Co_1\\Config\\NetElement";
	CMxHandleLoc handleLoc;
	char szError[500] = {0};
	int nRet = handleLoc.Treedb_Alloc(netConfig.m_strDBIp,"master","Co");
	if(nRet != BS_NOERROR){
		sprintf(szError,"master open failed: %d",nRet);
		strError = szError;
		return false;    		
	}

	nRet  = handleLoc.Treedb_ReopenSubKey(strSubKeyPath);
	if(nRet != BS_NOERROR){
		sprintf(szError,"%s open failed: %d",strSubKeyPath.c_str(),nRet);
		strError = szError;
		return false;
	}

	nIp = handleLoc.Treedb_GetPropertyInt("MainboardIp",-1);
	return true;
}


bool CDb::getLines(std::list<NETELEMENTLINE> &lstLines,string &strError)
{
	char szError[100] = {0};
	CMxHandleLoc mxHandleLoc;
	int nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopology","",ExistNodePath);
	if(nRet != BS_NOERROR)
	{
		sprintf(szError,"Topolopy open failed: %d",nRet);
		strError = szError;
		return false;
	}

	string selectMainKey = mxHandleLoc.Treedb_GetPropertyString("SelectMainKey","");

	if (!strcmp(selectMainKey.c_str(), "NetTopology"))
	{
		goto part2;
	}
	else if (!strcmp(selectMainKey.c_str(), "NetTopologyBack"))
	{
		nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopologyBack","",ExistNodePath);
		if(nRet != BS_NOERROR)
		{
			sprintf(szError,"Topolopy open failed: %d",nRet);
			strError = szError;
			return false;
		}
		goto part2;
	}

part2:

	nRet = mxHandleLoc.Treedb_ReopenSubKey("TopologyLines");
	if(nRet != BS_NOERROR)
	{
		if(nRet == BS_ERROR_BPTREENOFIND)
		{
			 // 当前没有线生成
			 return true;
		}
		sprintf(szError,"TopologyLines open failed:",nRet);
		strError = szError;
		return false;
	}

	char szLinePath[100] = {0};
	list<string> lstLineNames;
	nRet = mxHandleLoc.Treedb_GetAllSubKeys(lstLineNames);
	if(nRet != BS_NOERROR)
	{
		sprintf(szLinePath,"TopologyLines get failed: %d",nRet);
		strError = szError;
		return false;
	}

	 
	list<string>::iterator iter_LineName = lstLineNames.begin();
	for(;iter_LineName != lstLineNames.end();iter_LineName++)
	{
		sprintf(szLinePath,"TopologyLines\\%s",iter_LineName->c_str());
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			continue;
		}

		NETELEMENTLINE line;
		line.m_nStatus = mxHandleLoc.Treedb_GetPropertyInt("status",-1);
		line.m_strSrcId= mxHandleLoc.Treedb_GetPropertyString("SrcNodeId","");
		line.m_nSrcIndex = mxHandleLoc.Treedb_GetPropertyInt("SrcNodeInterface",-1);
		line.m_strDstId = mxHandleLoc.Treedb_GetPropertyString("DstNodeId","");
		line.m_nDstIndex = mxHandleLoc.Treedb_GetPropertyInt("DstNodeInterface",-1);

		line.m_nCost = mxHandleLoc.Treedb_GetPropertyInt("Costs",-1);
		line.m_nSelect = 0;
		line.m_strLineId=*iter_LineName;
		lstLines.push_back(line);
	} 

	//找到控制器起始ID
	std::list<NETELEMENTLINE>::iterator iter_lstLines = lstLines.begin();
	for (;iter_lstLines != lstLines.end(); iter_lstLines++ )
	{
		sprintf(szLinePath,"NetDevices\\%s",iter_lstLines->m_strSrcId.c_str());
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			return false;
		}
		iter_lstLines->m_nSrcDEVId = mxHandleLoc.Treedb_GetPropertyInt("devid",-1);
	}

	//找到控制器终点ID
	iter_lstLines = lstLines.begin();
	for (;iter_lstLines != lstLines.end(); iter_lstLines++ )
	{
		sprintf(szLinePath,"NetDevices\\%s",iter_lstLines->m_strDstId.c_str());
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			return false;
		}
		iter_lstLines->m_nDstDEVId = mxHandleLoc.Treedb_GetPropertyInt("devid",-1);
	}

	//m_nDstDEVId

	//循环容器，提取别数据
	iter_lstLines = lstLines.begin();
	for (;iter_lstLines!=lstLines.end(); iter_lstLines++)
	{
		sprintf(szLinePath,"NetDevices\\%s\\%d",iter_lstLines->m_strSrcId.c_str(),iter_lstLines->m_nSrcIndex);
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			return false;
		}

		iter_lstLines->m_oInterfaces.m_nChannelType = mxHandleLoc.Treedb_GetPropertyInt("ChannelType",-1);
		iter_lstLines->m_oInterfaces.m_nBandwidth = mxHandleLoc.Treedb_GetPropertyInt("Bandwidth",-1);
		iter_lstLines->m_oInterfaces.m_fBandwidthUtilization = mxHandleLoc.Treedb_GetPropertyFloat("BandwidthUtilization",1.0);

		//iter_lstLines->m_SrcPoint.m_strInterfaceIp = mxHandleLoc.Treedb_GetPropertyString("InterfaceIp","");
		iter_lstLines->m_SrcPoint.m_nstatus = mxHandleLoc.Treedb_GetPropertyInt("status",-1);
		iter_lstLines->m_SrcPoint.m_fErrorRate = mxHandleLoc.Treedb_GetPropertyFloat("ErrorRate",1.0);

		iter_lstLines->m_SrcPoint.m_njitter = mxHandleLoc.Treedb_GetPropertyInt("jitter",-1);
		iter_lstLines->m_SrcPoint.m_favgrtt = mxHandleLoc.Treedb_GetPropertyFloat("avgrtt",1.0);
		iter_lstLines->m_SrcPoint.m_fpacketloss = mxHandleLoc.Treedb_GetPropertyFloat("packetloss",1.0);

		sprintf(szLinePath,"NetDevices\\%s\\%d",iter_lstLines->m_strDstId.c_str(),iter_lstLines->m_nDstIndex);
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			return false;
		}

		//iter_lstLines->m_DstPoint.m_strInterfaceIp = mxHandleLoc.Treedb_GetPropertyString("InterfaceIp","");
		iter_lstLines->m_DstPoint.m_nstatus = mxHandleLoc.Treedb_GetPropertyInt("status",-1);
		iter_lstLines->m_DstPoint.m_fErrorRate = mxHandleLoc.Treedb_GetPropertyFloat("ErrorRate",1.0);

		iter_lstLines->m_DstPoint.m_njitter = mxHandleLoc.Treedb_GetPropertyInt("jitter",-1);
		iter_lstLines->m_DstPoint.m_favgrtt = mxHandleLoc.Treedb_GetPropertyFloat("avgrtt",1.0);
		iter_lstLines->m_DstPoint.m_fpacketloss = mxHandleLoc.Treedb_GetPropertyFloat("packetloss",1.0);
	}

	//重新取取起点IP地址
	lstLineNames.clear();
	iter_lstLines = lstLines.begin();
	for (;iter_lstLines!=lstLines.end(); iter_lstLines++)
	{
		sprintf(szLinePath,"NetDevices\\%s",iter_lstLines->m_strSrcId.c_str());
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			return false;
		}
		iter_lstLines->m_SrcPoint.m_strInterfaceIp = mxHandleLoc.Treedb_GetPropertyString("MainboardIp","");
	}

	//重新取取终点IP地址
	iter_lstLines = lstLines.begin();
	for (;iter_lstLines!=lstLines.end(); iter_lstLines++)
	{
		sprintf(szLinePath,"NetDevices\\%s",iter_lstLines->m_strDstId.c_str());
		nRet = mxHandleLoc.Treedb_ReopenSubKey(szLinePath);
		if(nRet != BS_NOERROR)
		{
			// 线读取失败
			return false;
		}
		iter_lstLines->m_DstPoint.m_strInterfaceIp = mxHandleLoc.Treedb_GetPropertyString("MainboardIp","");
	}

	return true;
}

bool CDb::getOptimalPath(list<OPTIMALPATH> &lstOptPath,string &strError)
{
	char szError[100] = {0};
	CMxHandleLoc mxHandleLoc;
	int nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopology","",ExistNodePath);
	if(nRet != BS_NOERROR)
	{
		sprintf(szError,"Topolopy open failed: %d",nRet);
		strError = szError;
		return false;
	}

	string selectMainKey = mxHandleLoc.Treedb_GetPropertyString("SelectMainKey","");

	if (!strcmp(selectMainKey.c_str(), "NetTopology"))
	{
		goto part2;
	}
	else if (!strcmp(selectMainKey.c_str(), "NetTopologyBack"))
	{
		nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopologyBack","",ExistNodePath);
		if(nRet != BS_NOERROR)
		{
			sprintf(szError,"Topolopy open failed: %d",nRet);
			strError = szError;
			return false;
		}
		goto part2;
	}

part2:

	nRet = mxHandleLoc.Treedb_ReopenSubKey("NetDevices");
	if(nRet != BS_NOERROR)
	{
		if(nRet == BS_ERROR_BPTREENOFIND)
		{
			 // 当前没有线生成
			 return true;
		}
		sprintf(szError,"NetDevices open failed:",nRet);
		strError = szError;
		return false;
	}
	
	char szLinePath[100] = {0};
	list<string> lstOpticalPath;
	nRet = mxHandleLoc.Treedb_GetAllSubKeys(lstOpticalPath);
	if(nRet != BS_NOERROR)
	{
		sprintf(szLinePath,"NetDevices get failed: %d",nRet);
		strError = szError;
		return false;
	}

	char szDevPath[500] = {0};
	list<string>::iterator iter_dev = lstOpticalPath.begin();
	for(;iter_dev != lstOpticalPath.end();iter_dev++)
	{
		string strDevSn = *iter_dev;
		sprintf(szDevPath,"%s\\%s","NetDevices",strDevSn.c_str());

		nRet = mxHandleLoc.Treedb_ReopenSubKey(szDevPath);
		if(nRet != BS_NOERROR)
		{
			continue;
		}

		int isActivity = mxHandleLoc.Treedb_GetPropertyInt("isActivity",0);
		if(isActivity == 1)
		{
			OPTIMALPATH optimakpath;
		//	optimakpath.m_strSN = *iter_dev;

			optimakpath.m_pathNumber = mxHandleLoc.Treedb_GetPropertyInt("PathNumber",-1);
			optimakpath.m_pathLinkNumber = mxHandleLoc.Treedb_GetPropertyInt("PathLinkNumber",-1);
			optimakpath.m_portOut = mxHandleLoc.Treedb_GetPropertyInt("DevOutport",-1);
			optimakpath.m_nextGecoid = mxHandleLoc.Treedb_GetPropertyInt("NextDevId",-1);
			optimakpath.m_nextGecoport = mxHandleLoc.Treedb_GetPropertyInt("NextDevInport",-1);

			lstOptPath.push_back(optimakpath);
		}
	}
	lstOpticalPath.clear();
	
	return true;
	
}


bool CDb::getLocalFault(list<LOCALFAULT> &lstLocalFau,string &strError)
{
	char szError[100] = {0};
	CMxHandleLoc mxHandleLoc;
	int nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopology","",ExistNodePath);
	if(nRet != BS_NOERROR)
	{
		sprintf(szError,"Topolopy open failed: %d",nRet);
		strError = szError;
		return false;
	}

	string selectMainKey = mxHandleLoc.Treedb_GetPropertyString("SelectMainKey","");

	if (!strcmp(selectMainKey.c_str(), "NetTopology"))
	{
		goto part2;
	}
	else if (!strcmp(selectMainKey.c_str(), "NetTopologyBack"))
	{
		nRet = mxHandleLoc.Treedb_Alloc(netConfig.m_strDBIp,"ccubase","NetTopologyBack","",ExistNodePath);
		if(nRet != BS_NOERROR)
		{
			sprintf(szError,"Topolopy open failed: %d",nRet);
			strError = szError;
			return false;
		}
		goto part2;
	}

part2:

	nRet = mxHandleLoc.Treedb_ReopenSubKey("NetDevices");
	if(nRet != BS_NOERROR)
	{
		if(nRet == BS_ERROR_BPTREENOFIND)
		{
			 // 当前没有线生成
			 return true;
		}
		sprintf(szError,"NetDevices open failed:",nRet);
		strError = szError;
		return false;
	}
	
	char szLinePath[100] = {0};
	list<string> lstLocalFault;
	nRet = mxHandleLoc.Treedb_GetAllSubKeys(lstLocalFault);
	if(nRet != BS_NOERROR)
	{
		sprintf(szLinePath,"NetDevices get failed: %d",nRet);
		strError = szError;
		return false;
	}

	char szDevPath[500] = {0};
	list<string>::iterator iter_dev = lstLocalFault.begin();
	for(;iter_dev != lstLocalFault.end();iter_dev++)
	{
		string strDevSn = *iter_dev;
		sprintf(szDevPath,"%s\\%s","NetDevices",strDevSn.c_str());

		nRet = mxHandleLoc.Treedb_ReopenSubKey(szDevPath);
		if(nRet != BS_NOERROR)
		{
			continue;
		}

		int isActivity = mxHandleLoc.Treedb_GetPropertyInt("isActivity",0);
		if(isActivity == 1)
		{
			LOCALFAULT localfal;
			localfal.m_strSN = *iter_dev;

			localfal.m_faultAlarmRecord = mxHandleLoc.Treedb_GetPropertyInt("ErrorNums",-1);
			localfal.m_faultAlarmId = mxHandleLoc.Treedb_GetPropertyInt("ErrorRecoId",-1);
			localfal.m_faultAlarmCode = mxHandleLoc.Treedb_GetPropertyInt("ErrorCodes",-1);
			localfal.m_faultStartTime = mxHandleLoc.Treedb_GetPropertyString("OccurTime","");
			localfal.m_faultRecoverTime = mxHandleLoc.Treedb_GetPropertyString("RecoverTime","");
			localfal.m_faultNameLen = mxHandleLoc.Treedb_GetPropertyInt("WarnTypeLen",-1);
			localfal.m_faultAlarmName = mxHandleLoc.Treedb_GetPropertyString("WarnTypeName","");
			localfal.m_faultAlarmLen = mxHandleLoc.Treedb_GetPropertyInt("WarnDescLen",-1);
			localfal.m_faultAlarmDescri = mxHandleLoc.Treedb_GetPropertyString("WarnDesc","");

			lstLocalFau.push_back(localfal);
		}
	}
	lstLocalFault.clear();
	
	return true;
	
}
