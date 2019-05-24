#ifndef LOCAL_CONTROLLER_STATE_H
#define LOCAL_CONTROLLER_STATE_H

#include <string>
using namespace std;
#include "../MXDatabase/NetElementDb.h"

class CLocalControllerState
{
public:
	CLocalControllerState();
	~CLocalControllerState();

	void Clear();
	void ClearCommand();
	void ClearResponse();
	bool CheckCommand(char* data);
	bool Response(int flowNumber);

	void* MyMemMove(void *dst,const void *src,int count);
public:
	bool GetGECO(list<DEVICE> &lstDevs, string &strError);           //获取通控器

	bool AddGECOID(DEVICE device);           //通控器ID
	bool AddGECONAME(DEVICE device);         //通控器名称
	bool AddEQCODING(DEVICE device);         //设备编码
	bool AddHARDWARE(DEVICE device);         //硬件版本
	bool AddSOFTWAR(DEVICE device);          //软件版本
	bool AddNETWORKIP(DEVICE device);        //网络IP地址
	bool AddMANAGEMENTIP(DEVICE device);     //管理IP地址
	bool AddEQUIPMENTSTATE(DEVICE device);   //设备状态
	bool AddLONGITUDECHAR(DEVICE device);    //经度字符(度、分、秒)
	bool AddLATITUDECHAR(DEVICE device);     //经度字符(度、分、秒) 
	bool AddCONTROLLERLEVEL(DEVICE device);  //控制器级别

private:
	char m_HEAD[6+1];   //帧头标志 HEAD
	char m_DA[1+1];     //信宿标识 DA
	char m_SA[1+1];     //信源标识 SA
	char m_LEN[2+1];    //帧长度 LEN
	char m_FT[1+1];     //帧命令类型 FT
	char m_SEQ[4+1];    //帧流水号 SEQ
	char m_FCS[2+1];    //帧校验 FCS

private:
	char m_GECOID[4+1];          //通控器ID
	char m_GECONAME[30+1];       //通控器名称
	char m_EQCODING[20+1];       //设备编码
	char m_HARDWARE[4+1];        //硬件版本
	char m_SOFTWARE[4+1];        //软件版本
	char m_NETWORKIP[4+1];       //网络IP地址
	char m_MANAGEMENTIP[4+1];    //管理IP地址
	char m_EQUIPMENTSTATE[4+1];  //设备状态
	char m_LONGITUDECHAR[1+1];   //经度字符
	//char m_LONGITUDEVALUE[8+1];  //经度值
	char m_LONGITUDEDEGRR[1+1];  //经度度
	char m_LONGITUDEPOINT[1+1];  //经度分
	char m_LONGITUDESECONDS[1+1];//经度秒
	char m_LATITUDECHAR[1+1];    //经度字符
	//char m_LATITUDEVALUE[7+1];   //经度值
	char m_LATITUDEDEGRR[1+1];   //经度度
	char m_LATITUDEPOINT[1+1];   //经度分
	char m_LATITUDESECONDS[1+1]; //经度秒
	char m_CONTROLLERLEVEL[1+1]; //控制器级别
public:
	char m_szResponsePackage[1600];
	int  m_nResponsePackageLen;
private:
	string m_strSN;  //通控器编号
public:
	void charIntToByte(int i, char *bytes);

};



#endif //LOCAL_CONTROLLER_STATE_H