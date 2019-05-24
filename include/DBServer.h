#ifndef _DBSERVER_H_
#define _DBSERVER_H_

#include <string>

using namespace std;

//同一台机器上可能会有多个数据库在运行，主机名和端口确定一个数据库运行实例
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

//进入表数据库的一种方式
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