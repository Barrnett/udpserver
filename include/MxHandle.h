#ifndef _MXHANDLE_H_
#define _MXHANDLE_H_

#include <string>
#include <list>
#include <map>
#include "bsapi.h"
#include "bserror.h"
#include "tabledb_def.h"
#include "treedb_def.h"
#include "BSMQDefine.h"

using namespace std;

#define TreedbHandle	0
#define TabledbHandle	1
#define MQdbHandle		2
#define UnknownHandle	-1

//huandle对象都是任意时刻都只有一个指针引用，所以不需要继承于CDealHandleObj
class CConfHandle
{
public:
	CConfHandle();
	virtual ~CConfHandle();
public:
	BSHANDLE m_hd;
};

class CTreedbHd: public CConfHandle
{
public:
	CTreedbHd();
	virtual ~CTreedbHd();
public:
	//string m_szMainKey;
	string m_szPwd;//主键的密码
	//string m_szSubKeyPath;
	int m_nOFlag;//打开键的方式

	bool m_bHaveTransaction;//是否有事务等待处理
};
class CTabledbHd: public CConfHandle
{
public:
	CTabledbHd();
	virtual ~CTabledbHd();
public:
	//int m_nOFlag;//打开数据库的方法
	//int m_nDBFlag;//如果创建此值指定数据库的风格

	bool m_bHaveTransaction;//是否有事务等待处理
};
class CMQHd: public CConfHandle
{
public:
	CMQHd();
	virtual ~CMQHd();
public:
	string m_szName;//MQ名称
	string m_szPwd;//打开此MQ所需密码
	//bool m_bIsValid;//指明当前保存的用户名和密码是否有效

	int m_nOFlag;//打开MQ的操作方式
	int m_nMQFlag;//如果是创建，此参数指定所创建MQ的风格
};

#endif