#ifndef NETWORK_INFORMATION_H
#define NETWORK_INFORMATION_H

#include <list>
#include "../MXDatabase/NetElementDb.h"
using namespace std;

class CNetworkInformation
{
public:
	CNetworkInformation();
	~CNetworkInformation();

	void Clear();
	void ClearCommand();
	void ClearResponse();

	bool CheckCommand(char* data);
	bool Response(int flowNumber);
	
	//正常应答
	bool ResponseNormal(int packetOrder);
	//否认应答
	bool ResponseDeny(int packetOrder);
	//获取数据库数据
	bool GetDataBase(); 
	//填充数据大小
	int  FillDataSize();
	//最大的分发包数
	int  MaxHandoutNum();
	//获取包头数据
	bool FillHeader(char *buffer, int &bufferLen);
	//获取包操作
	bool FillHandle(char *buffer, int &bufferLen);
	//获取包体数据
	bool FillData(NETELEMENTLINE elementline,char *buffer, int &bufferLen);
	//数据包验证
	bool FillVerify(int &bufferLen);

	
public:
	bool AddLinkState(NETELEMENTLINE line);            //链路状态
	bool AddLinkType(NETELEMENTLINE line);             //链路类型
	bool AddBandwidth(NETELEMENTLINE line);            //信道带宽
	bool AddBandwidthUtilization(NETELEMENTLINE line); //业务带宽占比
	bool AddSrcNodeId(NETELEMENTLINE line);            //起点控制器ID
	bool AddStartInterfaceIp(NETELEMENTLINE line);     //起点控制器IP
	bool AddSrcNodeInterface(NETELEMENTLINE line);     //起点控制器端口
	bool AddStartStatus(NETELEMENTLINE line);          //起点控制器状态
	bool AddStartErrorRate(NETELEMENTLINE line);       //起点接收误码率
	bool AddStartJitter(NETELEMENTLINE line);          //起点传输时延
	bool AddStartAvgrtt(NETELEMENTLINE line);          //起点传输抖动
	bool AddStartPacketloss(NETELEMENTLINE line);      //起点丢包率
	bool AddDstNodeId(NETELEMENTLINE line);            //终点控制器ID
	bool AddEndInterfaceIp(NETELEMENTLINE line);       //终点控制器IP
	bool AddDstNodeInterface(NETELEMENTLINE line);     //终点控制器端口
	bool AddEndStatus(NETELEMENTLINE line);            //终点控制器状态
	bool AddEndErrorRate(NETELEMENTLINE line);         //终点接收误码率
	bool AddEndJitter(NETELEMENTLINE line);            //终点传输时延
	bool AddEndAvgrtt(NETELEMENTLINE line);            //终点传输抖动
	bool AddEndPacketloss(NETELEMENTLINE line);        //终点丢包率 

private:
	char m_HEAD[6+1];   //帧头标志 HEAD
	char m_DA[1+1];     //信宿标识 DA
	char m_SA[1+1];     //信源标识 SA
	char m_LEN[2+1];    //帧长度 LEN
	char m_FT[1+1];     //帧命令类型 FT
	char m_SEQ[4+1];    //帧流水号 SEQ
	char m_MIMA[4+1];   //查询任务标识
	char m_MAPA[2+1];   //最大数据包长度
	char m_QUPA[2+1];   //本次查询包序号
	char m_FCS[2+1];    //帧校验 FCS
	
private:
	char m_ConfirmLogo[1+1];         //确认标识
	char m_ReplyLogo[4+1];           //应答标识
	char m_TaskToTal[2+1];           //任务数据总包数
	char m_RelplySequence[2+1];      //本次应答包序号
	char m_LinkNumber[2+1];          //本次上报链路数量M

private:
	char m_LinkState[1+1];           //链路状态
	char m_LinkType[1+1];            //链路类型
	char m_Bandwidth[1+1];           //信道带宽
	char m_BandwidthUtilization[1+1];//业务带宽占比
	char m_SrcNodeId[4+1];           //起点控制器ID
	char m_StartInterfaceIp[4+1];    //起点控制器IP
	char m_SrcNodeInterface[1+1];    //起点控制器端口
	char m_StartStatus[1+1];         //起点控制器状态
	char m_StartErrorRate[2+1];      //起点接收误码率
	char m_StartJitter[2+1];         //起点传输时延
	char m_StartAvgrtt[2+1];         //起点传输抖动
	char m_StartPacketloss[1+1];     //起点丢包率
	char m_DstNodeId[4+1];           //终点控制器ID
	char m_EndInterfaceIp[4+1];      //终点控制器IP
	char m_DstNodeInterface[1+1];    //终点控制器端口
	char m_EndStatus[1+1];           //终点控制器状态
	char m_EndErrorRate[2+1];        //终点接收误码率
	char m_EndJitter[2+1];           //终点传输时延
	char m_EndAvgrtt[2+1];           //终点传输抖动
	char m_EndPacketloss[1+1];       //终点丢包率 


	//
	char m_ExpePack[2+1];              //期望的包序号

public:
	char m_szResponsePackage[1600];
	int  m_nResponsePackageLen;

public:
	list<NETELEMENTLINE> m_listLines;
	int m_nSignPlace;  //帧的长度标记位
	int m_nFlowNumber; //发包流水号
	int m_nPacketOrder;//发包顺序
public:
	void charIntToByte(int i, char *bytes);
};

#endif //NETWORK_INFORMATION_H