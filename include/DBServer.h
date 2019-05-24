#ifndef _DBSERVER_H_
#define _DBSERVER_H_

#include <string>

using namespace std;

//ͬһ̨�����Ͽ��ܻ��ж�����ݿ������У��������Ͷ˿�ȷ��һ�����ݿ�����ʵ��
class CDBServer
{
public:
	string m_szHost;
	int m_nPort;
public:
	CDBServer(string szHost,int nPort);
	CDBServer();
	~CDBServer();

	bool operator<(const CDBServer& dbserver) const;
	bool operator==(const CDBServer& dbserver) const;
	CDBServer& operator=(const CDBServer& dbserver);
};

//��������ݿ��һ�ַ�ʽ
class CTableDbAccess
{
public:
	string m_szDbName;
	string m_szUser;
	string m_szPwd;
public:
	CTableDbAccess(string szDbName,string szUser,string szPwd);
	CTableDbAccess();
	~CTableDbAccess();

	bool operator<(const CTableDbAccess& tabledb) const;
	bool operator==(const CTableDbAccess& tabledb) const;
	CTableDbAccess& operator=(const CTableDbAccess& tabledb);
};

#endif