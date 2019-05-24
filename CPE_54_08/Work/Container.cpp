#include "Container.h"
#include <memory.h>

//CBasicClass��
//************************************************************************
// �� �� �� : CBasicClass
// ��    �� : ���캯��
//************************************************************************
CBasicClass::CBasicClass()
{
	Clear();
}

//************************************************************************
// �� �� �� : CBasicClass
// ��    �� : ��������
//************************************************************************
CBasicClass::~CBasicClass()
{
	Clear();
}

//************************************************************************
// �� �� �� : Clear
// ��    �� : ��ʼ������
// �� �� ֵ ��
//************************************************************************
void CBasicClass::Clear()
{
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	m_nBufferLen = 0;
	m_nFlowNumber = 0;
	m_strSourceIP = "";
	m_nSourcePort = 0;
}


//CContainer��
//************************************************************************
// �� �� �� : CContainer
// ��    �� : ���캯��
//************************************************************************
CContainer1::CContainer1()
{
	Clear();
}

//************************************************************************
// �� �� �� : CBasicClass
// ��    �� : ��������
//************************************************************************
CContainer1::~CContainer1()
{
	Clear();
}

//************************************************************************
// �� �� �� : Clear
// ��    �� : ��ʼ������
// �� �� ֵ ��
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

