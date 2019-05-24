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

	bool AddPathNum(OPTIMALPATH optimalPathinfo);           //·������M
	bool AddPathLinkNum(OPTIMALPATH optimalPathinfo);       //·����·����
	bool AddGecOutPort(OPTIMALPATH optimalPathinfo);        //���������˿�
	bool AddNextGecId(OPTIMALPATH optimalPathinfo);			//��һ��������ID
	bool AddNextGecInPort(OPTIMALPATH optimalPathinfo);//��һ����������˿�

	void charIntToByte(int i, char *bytes);

private:
	char m_HEAD[6+1];   //֡ͷ��־ HEAD
	char m_DA[1+1];     //���ޱ�ʶ DA
	char m_SA[1+1];     //��Դ��ʶ SA
	char m_LEN[2+1];    //֡���� LEN
	char m_FT[1+1];     //֡�������� FT
	char m_SEQ[4+1];    //֡��ˮ�� SEQ
	char m_TALO[4+1];   //�����ʶ
	char m_SCO[4+1];    //Դ������
	char m_PCO[4+1];    //Ŀ�Ŀ�����
	char m_FCS[2+1];    //֡У�� FCS
	
private:
	//char m_SrcNodeId[4+1];          //Դ������ID
	//char m_DstNodeId[4+1];          //Ŀ�Ŀ�����ID
	char m_PathNums[1+1];           //·������M
	char m_PathLineNums[1+1];       //·����·����N
	char m_NodeInterface[1+1];      //���������˿�
	char m_NextNodeId[4+1];         //��һ��������ID
	char m_NextNodeInterface[1+1];  //��һ����������˿�
	/*
	char m_PathLineNums[1+1];     //·����·����N
	char m_NodeInterface[1+1];    //���������˿�
	char m_NextNodeId[4+1];        //��һ��������ID
	char m_NextNodeInterface[1+1]; //��һ����������˿�
	*/
public:
	char m_szResponsePackage[256];
	int  m_nResponsePackageLen;

};

#endif //OPTIMAL_PATH_INFORMATION_H