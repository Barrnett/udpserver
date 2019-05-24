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
	
	//����Ӧ��
	bool ResponseNormal(int packetOrder);
	//����Ӧ��
	bool ResponseDeny(int packetOrder);
	//��ȡ���ݿ�����
	bool GetDataBase(); 
	//������ݴ�С
	int  FillDataSize();
	//���ķַ�����
	int  MaxHandoutNum();
	//��ȡ��ͷ����
	bool FillHeader(char *buffer, int &bufferLen);
	//��ȡ������
	bool FillHandle(char *buffer, int &bufferLen);
	//��ȡ��������
	bool FillData(NETELEMENTLINE elementline,char *buffer, int &bufferLen);
	//���ݰ���֤
	bool FillVerify(int &bufferLen);

	
public:
	bool AddLinkState(NETELEMENTLINE line);            //��·״̬
	bool AddLinkType(NETELEMENTLINE line);             //��·����
	bool AddBandwidth(NETELEMENTLINE line);            //�ŵ�����
	bool AddBandwidthUtilization(NETELEMENTLINE line); //ҵ�����ռ��
	bool AddSrcNodeId(NETELEMENTLINE line);            //��������ID
	bool AddStartInterfaceIp(NETELEMENTLINE line);     //��������IP
	bool AddSrcNodeInterface(NETELEMENTLINE line);     //���������˿�
	bool AddStartStatus(NETELEMENTLINE line);          //��������״̬
	bool AddStartErrorRate(NETELEMENTLINE line);       //������������
	bool AddStartJitter(NETELEMENTLINE line);          //��㴫��ʱ��
	bool AddStartAvgrtt(NETELEMENTLINE line);          //��㴫�䶶��
	bool AddStartPacketloss(NETELEMENTLINE line);      //��㶪����
	bool AddDstNodeId(NETELEMENTLINE line);            //�յ������ID
	bool AddEndInterfaceIp(NETELEMENTLINE line);       //�յ������IP
	bool AddDstNodeInterface(NETELEMENTLINE line);     //�յ�������˿�
	bool AddEndStatus(NETELEMENTLINE line);            //�յ������״̬
	bool AddEndErrorRate(NETELEMENTLINE line);         //�յ����������
	bool AddEndJitter(NETELEMENTLINE line);            //�յ㴫��ʱ��
	bool AddEndAvgrtt(NETELEMENTLINE line);            //�յ㴫�䶶��
	bool AddEndPacketloss(NETELEMENTLINE line);        //�յ㶪���� 

private:
	char m_HEAD[6+1];   //֡ͷ��־ HEAD
	char m_DA[1+1];     //���ޱ�ʶ DA
	char m_SA[1+1];     //��Դ��ʶ SA
	char m_LEN[2+1];    //֡���� LEN
	char m_FT[1+1];     //֡�������� FT
	char m_SEQ[4+1];    //֡��ˮ�� SEQ
	char m_MIMA[4+1];   //��ѯ�����ʶ
	char m_MAPA[2+1];   //������ݰ�����
	char m_QUPA[2+1];   //���β�ѯ�����
	char m_FCS[2+1];    //֡У�� FCS
	
private:
	char m_ConfirmLogo[1+1];         //ȷ�ϱ�ʶ
	char m_ReplyLogo[4+1];           //Ӧ���ʶ
	char m_TaskToTal[2+1];           //���������ܰ���
	char m_RelplySequence[2+1];      //����Ӧ������
	char m_LinkNumber[2+1];          //�����ϱ���·����M

private:
	char m_LinkState[1+1];           //��·״̬
	char m_LinkType[1+1];            //��·����
	char m_Bandwidth[1+1];           //�ŵ�����
	char m_BandwidthUtilization[1+1];//ҵ�����ռ��
	char m_SrcNodeId[4+1];           //��������ID
	char m_StartInterfaceIp[4+1];    //��������IP
	char m_SrcNodeInterface[1+1];    //���������˿�
	char m_StartStatus[1+1];         //��������״̬
	char m_StartErrorRate[2+1];      //������������
	char m_StartJitter[2+1];         //��㴫��ʱ��
	char m_StartAvgrtt[2+1];         //��㴫�䶶��
	char m_StartPacketloss[1+1];     //��㶪����
	char m_DstNodeId[4+1];           //�յ������ID
	char m_EndInterfaceIp[4+1];      //�յ������IP
	char m_DstNodeInterface[1+1];    //�յ�������˿�
	char m_EndStatus[1+1];           //�յ������״̬
	char m_EndErrorRate[2+1];        //�յ����������
	char m_EndJitter[2+1];           //�յ㴫��ʱ��
	char m_EndAvgrtt[2+1];           //�յ㴫�䶶��
	char m_EndPacketloss[1+1];       //�յ㶪���� 


	//
	char m_ExpePack[2+1];              //�����İ����

public:
	char m_szResponsePackage[1600];
	int  m_nResponsePackageLen;

public:
	list<NETELEMENTLINE> m_listLines;
	int m_nSignPlace;  //֡�ĳ��ȱ��λ
	int m_nFlowNumber; //������ˮ��
	int m_nPacketOrder;//����˳��
public:
	void charIntToByte(int i, char *bytes);
};

#endif //NETWORK_INFORMATION_H