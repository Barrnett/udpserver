/* =================================================================== */
/*				 Copyright (C) 2007 MDCChina  CO., LTD.				   */
/* =================================================================== */
/*
NAME
	dbsend.cpp

HISTORY
	2007/08/28         lwhan      Create
		- source for dbsend
*/
#include "DbSend.h"
extern NetConfig netConfig;

THREAD_TYPE DbSendThread(void *lpPara)
{
	CDbSend *pThread = (CDbSend*)lpPara;
	pThread->m_nState = 1;
	if (pThread == NULL)
	{
		printf("%s:%d,failed,pThread=null!!\n",__FILE__,__LINE__);
		pThread->m_nState = 2;
		return (void *)-1;
	}

	pThread->m_bIsStart = true;
	//192.168.1.106 6901
	if (OK != pThread->socketUDP.OpenUDPClient(pThread->m_pBasicClass->m_strSourceIP.c_str(),netConfig.m_nClientPort))
	{
		printf("%s:%d,failed,create socket failed!\n",__FILE__,__LINE__);
		pThread->m_nState = 2;
		return (void *)-1;
	}
	//��֤��֡����ˮ�ţ�
	char flowNumber[5];
	memset(flowNumber, 0, sizeof(flowNumber));
	memcpy(flowNumber, pThread->m_pBasicClass->m_szBuffer + 11, 4);

	//��ȡ֡����������
	int nameType = pThread->byteToInt(pThread->m_pBasicClass->m_szBuffer[10]);
	switch (nameType)
	{
	case 1:
		{
			pThread->m_LocalControllerState.CheckCommand(pThread->m_pBasicClass->m_szBuffer);
			bool temp = pThread->m_LocalControllerState.Response(pThread->m_pBasicClass->m_nFlowNumber);
			char *responsePackage = pThread->m_LocalControllerState.m_szResponsePackage;
			int responsePackageLen = pThread->m_LocalControllerState.m_nResponsePackageLen;
			temp = pThread->SendTheckPack(responsePackage, responsePackageLen);
			if (temp)
			{
				//�ɹ�
			}
			else
			{
				//ʧ��
			}
			break;
		}
	case 2:
		{
			pThread->m_LocalControllerList.CheckCommand(pThread->m_pBasicClass->m_szBuffer);
			bool temp = pThread->m_LocalControllerList.Response(pThread->m_pBasicClass->m_nFlowNumber);
			char *responsePackage = pThread->m_LocalControllerList.m_szResponsePackage;
			int responsePackageLen = pThread->m_LocalControllerList.m_nResponsePackageLen;
			temp = pThread->SendTheckPack(responsePackage, responsePackageLen);
			if (temp)
			{
				//�ɹ�
			}
			else
			{
				//ʧ��
			}
			break;
		}
	case 3:
		{
			pThread->m_NetworkInformation.CheckCommand(pThread->m_pBasicClass->m_szBuffer);
			bool temp = pThread->m_NetworkInformation.Response(pThread->m_pBasicClass->m_nFlowNumber);
			char *responsePackage = pThread->m_NetworkInformation.m_szResponsePackage;
			int responsePackageLen = pThread->m_NetworkInformation.m_nResponsePackageLen;
			temp = pThread->SendTheckPack(responsePackage, responsePackageLen);
			if (temp)
			{
				//�ɹ�
			}
			else
			{
				//ʧ��
			}
		
			break;
		
		}
	case 4:
		{
			pThread->m_OptimalPathInformation.CheckCommand(pThread->m_pBasicClass->m_szBuffer);
			bool temp = pThread->m_OptimalPathInformation.Response(pThread->m_pBasicClass->m_nFlowNumber);
			char *responsePackage = pThread->m_OptimalPathInformation.m_szResponsePackage;
			int responsePackageLen = pThread->m_OptimalPathInformation.m_nResponsePackageLen;
			temp = pThread->SendTheckPack(responsePackage, responsePackageLen);
			if (temp)
			{
				//�ɹ�
			}
			else
			{
				//ʧ��
			}
			break;
		}
	case 5:
		{
			pThread->m_LocalFault.CheckCommand(pThread->m_pBasicClass->m_szBuffer);
			bool temp = pThread->m_LocalFault.Response(pThread->m_pBasicClass->m_nFlowNumber);
			char *responsePackage = pThread->m_LocalFault.m_szResponsePackage;
			int responsePackageLen = pThread->m_LocalFault.m_nResponsePackageLen;
			temp = pThread->SendTheckPack(responsePackage, responsePackageLen);
			if (temp)
			{
				//�ɹ�
			}
			else
			{
				//ʧ��
			}
			break;
		}
	}

	pThread->CloseTheckPack();
	pThread->m_nState = 2;

	return (void *)0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbSend::CDbSend():CThread(DbSendThread)
{
	m_nNetworkMarkeNumber = 0;
	m_pBasicClass = NULL;
	m_nState = 0;
}

CDbSend::~CDbSend()
{
	delete m_pBasicClass;
	m_pBasicClass = NULL;
}


bool CDbSend::Initialize()
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

void CDbSend::UnInitialize()
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
*������ : SendTheckPack
*������������:����UDP��֤ȷ�ϰ�
*�������� : 
*��������ֵ : true-�ɹ� false-ʧ��
***********************************************************************/
bool CDbSend::SendTheckPack(char* buffer, int nBufLen)
{
	if (socketUDP.SendUDPClient(buffer, nBufLen) != OK)
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
bool CDbSend::CloseTheckPack()
{
	
	if (socketUDP.CloseUDP(socketUDP.GetClientUDPSocket()) != OK)
	{
		return false;
	}
	return true;
}

//bytesת����Int
int CDbSend::bytesToInt(char* bytes)
{
	int a = bytes[3] & 0xFF;
	a |= ((bytes[2] << 8) & 0xFF00);
	a |= ((bytes[1] << 16) & 0xFF0000);
	a |= ((bytes[0] << 24) & 0xFF000000);

	return a;
}

int CDbSend::byteToInt(char byte)
{
	int a = byte & 0xFF;
	return a;
}

//16���Ƶ��ı��ַ�����ת��
void CDbSend::HexStrToByte(const char* source, unsigned char* dest, int sourceLen) 
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

void CDbSend::setData(CBasicClass * pBasicData)
{
	m_pBasicClass = pBasicData;
	return;
}
