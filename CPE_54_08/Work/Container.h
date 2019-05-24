#ifndef CONTAINER_H
#define CONTAINER_H
#include <string>
#include <queue>
using namespace std;

class CBasicClass
{
public:
	CBasicClass();
	~CBasicClass();
	void Clear();
protected:
private:
public:
	char m_szBuffer[512];      //����
	int  m_nBufferLen;         //���ĳ���
	int  m_nFlowNumber;        //������ˮ��
	string m_strSourceIP;      //����ԴIP
	int m_nSourcePort;         //����Դ�˿�
};

class CContainer1
{
public:
	CContainer1();
	~CContainer1();
	void Clear();

public:
	bool PushExecute(CBasicClass *basicObject);
	CBasicClass* FrontExecute();
	bool PopExecute();

private:
	std::queue<CBasicClass*> m_queueBuffer;
};

#endif //CONTAINER_H