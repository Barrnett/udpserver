#ifndef OPTIMAL_PATH_INFORMATION_H
#define OPTIMAL_PATH_INFORMATION_H

#include <string>
using namespace std;
#include "../MXDatabase/NetElementDb.h"

class COptimalPathInformation
{
public:
	COptimalPathInformation();
	~COptimalPathInformation();

	void Clear();
	bool CheckCommand(char* data);
	bool Response(int flowNumber);
	
	bool getOptimalPath(list<OPTIMALPATH> &lstOptPath,string &strError);

	bool AddPathNum(OPTIMALPATH optimalPathinfo);           //路径数量M
	bool AddPathLinkNum(OPTIMALPATH optimalPathinfo);       //路径链路数量
	bool AddGecOutPort(OPTIMALPATH optimalPathinfo);        //控制器出端口
	bool AddNextGecId(OPTIMALPATH optimalPathinfo);			//下一跳控制器ID
	bool AddNextGecInPort(OPTIMALPATH optimalPathinfo);//下一跳控制器入端口

	void charIntToByte(int i, char *bytes);

private:
	char m_HEAD[6+1];   //帧头标志 HEAD
	char m_DA[1+1];     //信宿标识 DA
	char m_SA[1+1];     //信源标识 SA
	char m_LEN[2+1];    //帧长度 LEN
	char m_FT[1+1];     //帧命令类型 FT
	char m_SEQ[4+1];    //帧流水号 SEQ
	char m_TALO[4+1];   //任务标识
	char m_SCO[4+1];    //源控制器
	char m_PCO[4+1];    //目的控制器
	char m_FCS[2+1];    //帧校验 FCS
	
private:
	//char m_SrcNodeId[4+1];          //源控制器ID
	//char m_DstNodeId[4+1];          //目的控制器ID
	char m_PathNums[1+1];           //路径数量M
	char m_PathLineNums[1+1];       //路径链路数量N
	char m_NodeInterface[1+1];      //控制器出端口
	char m_NextNodeId[4+1];         //下一跳控制器ID
	char m_NextNodeInterface[1+1];  //下一跳控制器入端口
	/*
	char m_PathLineNums[1+1];     //路径链路数量N
	char m_NodeInterface[1+1];    //控制器出端口
	char m_NextNodeId[4+1];        //下一跳控制器ID
	char m_NextNodeInterface[1+1]; //下一跳控制器入端口
	*/
public:
	char m_szResponsePackage[256];
	int  m_nResponsePackageLen;

};

#endif //OPTIMAL_PATH_INFORMATION_H