#ifndef LOCAL_FAULT_H
#define LOCAL_FAULT_H

#include <string>
using namespace std;
#include "../MXDatabase/NetElementDb.h"

class CLocalFault
{
public:
	CLocalFault();
	~CLocalFault();
	void Clear();

	bool CheckCommand(char* data);
	bool Response(int flowNumber);
	void  charIntToByte(int i, char *bytes);
	
	bool LocalFauNums(LOCALFAULT localfauinfo);
	bool FaultRecoId(LOCALFAULT localfauinfo);
	bool FaultTypeCode(LOCALFAULT localfauinfo);
	bool FaultTime(LOCALFAULT localfauinfo);
	bool FaultRecoTime(LOCALFAULT localfauinfo);
	bool WarnTypeLen(LOCALFAULT localfauinfo);
	bool WarnTypeName(LOCALFAULT localfauinfo);
	bool WarnDescLen(LOCALFAULT localfauinfo);
	bool WarnDescri(LOCALFAULT localfauinfo);
	
private:
	char m_HEAD[6+1];   //֡ͷ��־ HEAD
	char m_DA[1+1];     //���ޱ�ʶ DA
	char m_SA[1+1];     //��Դ��ʶ SA
	char m_LEN[2+1];    //֡���� LEN
	char m_FT[1+1];     //֡�������� FT
	char m_SEQ[4+1];    //֡��ˮ�� SEQ
	char m_FCS[2+1];    //֡У�� FCS

	//
	char m_ErroeReportNums[1+1];    //���ι��ϸ澯��¼����M
    char m_ErrorReportId[4+1];      //���ϸ澯��¼ID
	char m_ErrorType[4+1];          //���ϸ澯���ͱ���
	char m_OccurTime[5+1];          //����ʱ��
	char m_RecoverTime[5+1];        //�ָ�ʱ��
	char m_ErrorTypeNameLen[1+1];   //�澯�������Ƴ���
	char m_ErrorTypeName[32+1];     //�澯��������
	char m_ErrorDesLen[1+1];        //�澯��������
	char m_ErrorDes[32+1];          //�澯����

public:
	char m_szResponsePackage[256];
	int  m_nResponsePackageLen;

};
#endif  //LOCAL_FAULT_H