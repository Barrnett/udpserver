/* =================================================================== */
/*				 Copyright (C) 2007 MDCChina  CO., LTD.				   */
/* =================================================================== */
/*
NAME
	dbread.h

HISTORY
	2007/09/28        lwhan      Create
		- header for dbread
*/
#if !defined(AFX_DBREAD_H__86A44E48_FDC8_4787_B621_1F9576B5B263__INCLUDED_)
#define AFX_DBREAD_H__86A44E48_FDC8_4787_B621_1F9576B5B263__INCLUDED_

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
#include "fileRead.h"

#ifdef WIN32
#define	THREAD_TYPE DWORD WINAPI
#else
#define	THREAD_TYPE void *
#endif


extern "C"
{
	THREAD_TYPE DbReadThread(void * lpPara);
}

class CDbRead : public CThread  
{
	friend THREAD_TYPE DbReadThread(void * lpPara);
public:
	CDbRead();
	virtual ~CDbRead();

	//bool m_CanGetIsagUser;

	bool Initialize();
	void UnInitialize();
	bool RecvTheckPack();
	bool SendTheckPack(char* buffer, int nBufLen);
	bool CloseTheckPack();

	//����
	int byteToInt(char byte);
	void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

private:
	CSocketUDP socketUDP;
	//����˶˿ں�
	char m_szUDPBuf[256];
	//����UDP���Ӱ���Ϣ���ݳ���
	int m_nUDPBufLen;
	//����UDP���Ӱ���Ϣ��ˮ��
	int m_nFlowNumber;

	//Э����
	CLocalControllerState m_LocalControllerState;    //����ͨ����״̬
	CLocalControllerList m_LocalControllerList;      //����ͨ�����б�
	CNetworkInformation m_NetworkInformation;        //ȫ��������Ϣ
	COptimalPathInformation m_OptimalPathInformation;//����·����Ϣ

};

#endif // !defined(AFX_DBREAD_H__86A44E48_FDC8_4787_B621_1F9576B5B263__INCLUDED_)
