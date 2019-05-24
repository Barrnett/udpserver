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

//huandle����������ʱ�̶�ֻ��һ��ָ�����ã����Բ���Ҫ�̳���CDealHandleObj
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
	string m_szPwd;//����������
	//string m_szSubKeyPath;
	int m_nOFlag;//�򿪼��ķ�ʽ

	bool m_bHaveTransaction;//�Ƿ�������ȴ�����
};
class CTabledbHd: public CConfHandle
{
public:
	CTabledbHd();
	virtual ~CTabledbHd();
public:
	//int m_nOFlag;//�����ݿ�ķ���
	//int m_nDBFlag;//���������ֵָ�����ݿ�ķ��

	bool m_bHaveTransaction;//�Ƿ�������ȴ�����
};
class CMQHd: public CConfHandle
{
public:
	CMQHd();
	virtual ~CMQHd();
public:
	string m_szName;//MQ����
	string m_szPwd;//�򿪴�MQ��������
	//bool m_bIsValid;//ָ����ǰ������û����������Ƿ���Ч

	int m_nOFlag;//��MQ�Ĳ�����ʽ
	int m_nMQFlag;//����Ǵ������˲���ָ��������MQ�ķ��
};

#endif