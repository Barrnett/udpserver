#include "Container.h"
#include <memory.h>

//CBasicClass类
//************************************************************************
// 函 数 名 : CBasicClass
// 描    述 : 构造函数
//************************************************************************
CBasicClass::CBasicClass()
{
	Clear();
}

//************************************************************************
// 函 数 名 : CBasicClass
// 描    述 : 析构函数
//************************************************************************
CBasicClass::~CBasicClass()
{
	Clear();
}

//************************************************************************
// 函 数 名 : Clear
// 描    述 : 初始化变量
// 返 回 值 ：
//************************************************************************
void CBasicClass::Clear()
{
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	m_nBufferLen = 0;
	m_nFlowNumber = 0;
	m_strSourceIP = "";
	m_nSourcePort = 0;
}


//CContainer类
//************************************************************************
// 函 数 名 : CContainer
// 描    述 : 构造函数
//************************************************************************
CContainer1::CContainer1()
{
	Clear();
}

//************************************************************************
// 函 数 名 : CBasicClass
// 描    述 : 析构函数
//************************************************************************
CContainer1::~CContainer1()
{
	Clear();
}

//************************************************************************
// 函 数 名 : Clear
// 描    述 : 初始化变量
// 返 回 值 ：
//************************************************************************
void CContainer1::Clear()
{
	queue<CBasicClass*> empty; 
	swap(empty,m_queueBuffer);
}

bool CContainer1::PushExecute(CBasicClass *basicObject)
{
	if (basicObject)
	{
		m_queueBuffer.push(basicObject);
		return true;
	}
	return false;
}

CBasicClass* CContainer1::FrontExecute()
{
	if (!m_queueBuffer.empty())
	{
		return m_queueBuffer.front();
	}
	return NULL;
}
bool CContainer1::PopExecute()
{
	if (!m_queueBuffer.empty())
	{
		m_queueBuffer.pop();
		return true;
	}
	return false;
}

