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
	char m_szBuffer[512];      //名称
	int  m_nBufferLen;         //包的长度
	int  m_nFlowNumber;        //包的流水号
	string m_strSourceIP;      //包来源IP
	int m_nSourcePort;         //包来源端口
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