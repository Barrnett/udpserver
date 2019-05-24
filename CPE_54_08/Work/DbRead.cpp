/* =================================================================== */
/*				 Copyright (C) 2007 MDCChina  CO., LTD.				   */
/* =================================================================== */
/*
NAME
	dbread.cpp

HISTORY
	2007/09/28         lwhan      Create
		- source for dbread
*/
#include "DbRead.h"
#include<assert.h>
#include "Container.h"
#include "../Tool/YCodec.h"
#include "TaskManager.h"
#include "DbSend.h"

static int cou;

extern Task taskDispatch;
extern NetConfig netConfig;

THREAD_TYPE DbReadThread(void * lpPara)
{
	CDbRead *pThread = (CDbRead*)lpPara;
	if (pThread == NULL)
	{
		printf("%s:%d,failed,pThread = null!\n",__FILE__,__LINE__);
		return (void *)-1;
	}

	pThread->m_bIsStart = true;
	//192.168.1.230 6901
	if (OK != pThread->socketUDP.OpenUDPServer(netConfig.m_strServIp.c_str(), netConfig.m_nServPort))
	{
		cout<<netConfig.m_strServIp<<netConfig.m_nServPort<<endl;
		printf("%s:%d,failed,open socket failed!\n",__FILE__,__LINE__);
		return (void *)-1;
	}

	while(!pThread->m_bExit)
	{
		//ѭ��ȥ����������
		if (pThread->RecvTheckPack())
		{
			//��֤�������յ���ͬ�İ���
			int flowNumber = YCodec::BytesToInt(pThread->m_szUDPBuf + 11);

			if (flowNumber != pThread->m_nFlowNumber)
			{
				pThread->m_nFlowNumber = flowNumber;
			}
			else
			{
				//��������ͬ�İ�
				continue;
			}
			
			CBasicClass *basicClass = new CBasicClass();
			memcpy(basicClass->m_szBuffer, pThread->m_szUDPBuf, pThread->m_nUDPBufLen);
			basicClass->m_nBufferLen = pThread->m_nUDPBufLen;
			basicClass->m_nFlowNumber = pThread->m_nFlowNumber;
			basicClass->m_strSourceIP = pThread->socketUDP.GetGuestUDPIpAddr();
			basicClass->m_nSourcePort = pThread->socketUDP.GetGuestUDPPort();

			CDbSend *pSendTask = new CDbSend();
			pSendTask->setData(basicClass);
			taskDispatch.setTask(pSendTask);
		}
		usleep(1000000);
	}

	pThread->CloseTheckPack();

	return (void *)0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbRead::CDbRead():CThread(DbReadThread)
{
	m_nUDPBufLen = 0;
	m_nFlowNumber = 0;
	memset(m_szUDPBuf, 0, sizeof(m_szUDPBuf));
}

CDbRead::~CDbRead()
{
	
}

bool CDbRead::Initialize()
{	
	m_bInit = true;

	//����UDP�������ˣ�������Ip��ַ�Ͷ˿ںţ�
	if(!start())
	{
		UnInitialize();
		return false;
	}

	return true;
}

void CDbRead::UnInitialize()
{
	if(!m_bInit) 
		return;
	if(isStart())
	{
		//ֹͣ�߳�
		stop();
	}
}

/**********************************************************************
*������ : RecvTheckPack
*������������:����UDP��֤ȷ�ϰ�
*�������� : 
*��������ֵ : true-�ɹ� false-ʧ��
***********************************************************************/
bool CDbRead::RecvTheckPack()
{
	//���տͻ���ȷ�ϰ�(ȷ�ϰ�����һ�ν�������)
	memset(m_szUDPBuf, 0, sizeof(m_szUDPBuf));
	if ((socketUDP.RecvUDPServer(m_szUDPBuf, m_nUDPBufLen) != OK) || m_nUDPBufLen <= 0)
	{
		return false;
	}
	//cout<<"This is a simple C++ program"<<endl;
	return true;
}

/**********************************************************************
*������ : SendTheckPack
*������������:����UDP��֤ȷ�ϰ�
*�������� : 
*��������ֵ : true-�ɹ� false-ʧ��
***********************************************************************/
bool CDbRead::SendTheckPack(char* buffer, int nBufLen)
{
	if (socketUDP.SendUDPServer(socketUDP.GetClientUDPSocket(), buffer, nBufLen) != OK)
	{
		return false;
	}
	return true;
}


/**********************************************************************
*������ : CloseTheckPack
*������������:�ر�socket
*�������� : 
*��������ֵ : true-�ɹ� false-ʧ��
***********************************************************************/
bool CDbRead::CloseTheckPack()
{
	if (socketUDP.CloseUDP(socketUDP.GetServerUDPSocket()) != OK)
	{
		return false;
	}
	return true;
}


int CDbRead::byteToInt(char byte)
{
	int a = byte & 0xFF;
	return a;
}

//16���Ƶ��ı��ַ�����ת��
void CDbRead::HexStrToByte(const char* source, unsigned char* dest, int sourceLen) 
{ 
	short i; 
	unsigned char highByte, lowByte; 

	for (i = 0; i < sourceLen; i += 2) 
	{ 
		highByte = toupper(source[i]); 
		lowByte = toupper(source[i + 1]); 

		if (highByte > 0x39) 
			highByte -= 0x37; 
		else 
			highByte -= 0x30; 

		if (lowByte > 0x39) 
			lowByte -= 0x37; 
		else 
			lowByte -= 0x30; 

		dest[i / 2] = (highByte << 4) | lowByte; 
	} 
	return ; 
}
