/* =================================================================== */
/*				 Copyright (C) 2007 MDCChina  CO., LTD.				   */
/* =================================================================== */
/*
NAME
	dbsend.h

HISTORY
	2007/09/28        lwhan      Create
		- header for dbpsend
*/
#if !defined(AFX_DBSEND_H__7CFE70BC_7053_4616_8F5D_7C2400A5DF7D__INCLUDED_)
#define AFX_DBSEND_H__7CFE70BC_7053_4616_8F5D_7C2400A5DF7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "../Thread/Thread.h"
#include "../Socket/UDP.h"
#include "../Business/LocalControllerState.h"
#include "../Business/LocalControllerList.h"
#include "../Business/NetworkInformation.h"
#include "../Business/OptimalPathInformation.h"
#include "../Business/LocalFault.h"
#include "fileRead.h"
#include "Container.h"

#ifdef WIN32
#define	THREAD_TYPE DWORD WINAPI
#else
#define	THREAD_TYPE void *
#endif


extern "C"
{
	THREAD_TYPE DbSendThread(void * lpPara);
}

class CDbSend : public CThread  
{
	friend THREAD_TYPE DbSendThread(void * lpPara);
public:
	CDbSend();
	virtual ~CDbSend();

	bool Initialize();
	void UnInitialize();

	bool SendTheckPack(char* buffer, int nBufLen);
	bool CloseTheckPack();

	int bytesToInt(char* bytes);
	int byteToInt(char byte);
	void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);
	
	void setData(CBasicClass * pBasicData);
public:
	int m_nState;//����״̬��0δ��ʼ��1���ڴ���2����ɡ�

private:
	int  m_nNetworkMarkeNumber;
	CSocketUDP socketUDP;
	CBasicClass *m_pBasicClass;

	//Э����
	CLocalControllerState m_LocalControllerState;    //����ͨ����״̬
	CLocalControllerList m_LocalControllerList;      //����ͨ�����б�
	CNetworkInformation m_NetworkInformation;        //ȫ��������Ϣ
	COptimalPathInformation m_OptimalPathInformation;//����·����Ϣ
	CLocalFault m_LocalFault;                        //���ع��ϸ澯��Ϣ
};

#endif // !defined(AFX_DBSEND_H__7CFE70BC_7053_4616_8F5D_7C2400A5DF7D__INCLUDED_)
