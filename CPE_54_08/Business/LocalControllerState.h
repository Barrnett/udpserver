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
	bool GetGECO(list<DEVICE> &lstDevs, string &strError);           //��ȡͨ����

	bool AddGECOID(DEVICE device);           //ͨ����ID
	bool AddGECONAME(DEVICE device);         //ͨ��������
	bool AddEQCODING(DEVICE device);         //�豸����
	bool AddHARDWARE(DEVICE device);         //Ӳ���汾
	bool AddSOFTWAR(DEVICE device);          //����汾
	bool AddNETWORKIP(DEVICE device);        //����IP��ַ
	bool AddMANAGEMENTIP(DEVICE device);     //����IP��ַ
	bool AddEQUIPMENTSTATE(DEVICE device);   //�豸״̬
	bool AddLONGITUDECHAR(DEVICE device);    //�����ַ�(�ȡ��֡���)
	bool AddLATITUDECHAR(DEVICE device);     //�����ַ�(�ȡ��֡���) 
	bool AddCONTROLLERLEVEL(DEVICE device);  //����������

private:
	char m_HEAD[6+1];   //֡ͷ��־ HEAD
	char m_DA[1+1];     //���ޱ�ʶ DA
	char m_SA[1+1];     //��Դ��ʶ SA
	char m_LEN[2+1];    //֡���� LEN
	char m_FT[1+1];     //֡�������� FT
	char m_SEQ[4+1];    //֡��ˮ�� SEQ
	char m_FCS[2+1];    //֡У�� FCS

private:
	char m_GECOID[4+1];          //ͨ����ID
	char m_GECONAME[30+1];       //ͨ��������
	char m_EQCODING[20+1];       //�豸����
	char m_HARDWARE[4+1];        //Ӳ���汾
	char m_SOFTWARE[4+1];        //����汾
	char m_NETWORKIP[4+1];       //����IP��ַ
	char m_MANAGEMENTIP[4+1];    //����IP��ַ
	char m_EQUIPMENTSTATE[4+1];  //�豸״̬
	char m_LONGITUDECHAR[1+1];   //�����ַ�
	//char m_LONGITUDEVALUE[8+1];  //����ֵ
	char m_LONGITUDEDEGRR[1+1];  //���ȶ�
	char m_LONGITUDEPOINT[1+1];  //���ȷ�
	char m_LONGITUDESECONDS[1+1];//������
	char m_LATITUDECHAR[1+1];    //�����ַ�
	//char m_LATITUDEVALUE[7+1];   //����ֵ
	char m_LATITUDEDEGRR[1+1];   //���ȶ�
	char m_LATITUDEPOINT[1+1];   //���ȷ�
	char m_LATITUDESECONDS[1+1]; //������
	char m_CONTROLLERLEVEL[1+1]; //����������
public:
	char m_szResponsePackage[1600];
	int  m_nResponsePackageLen;
private:
	string m_strSN;  //ͨ�������
public:
	void charIntToByte(int i, char *bytes);

};



#endif //LOCAL_CONTROLLER_STATE_H