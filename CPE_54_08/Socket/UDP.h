#ifndef _SOCKET_UDP_H
#define _SOCKET_UDP_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifndef OK
#define OK      0
#endif

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define SYS_ERROR_UDP_GENERAL                -200
#define SYS_ERROR_UDP_INITIALIZE             ( SYS_ERROR_UDP_GENERAL - 1 )
#define SYS_ERROR_UDP_CREATE                 ( SYS_ERROR_UDP_GENERAL - 2 )
#define SYS_ERROR_UDP_MODEL                  ( SYS_ERROR_UDP_GENERAL - 3 )
#define SYS_ERROR_UDP_BIND                   ( SYS_ERROR_UDP_GENERAL - 4 )
#define SYS_ERROR_UDP_TIMEOUT                ( SYS_ERROR_UDP_GENERAL - 5 )
#define SYS_ERROR_UDP_SEND                   ( SYS_ERROR_UDP_GENERAL - 6 )
#define SYS_ERROR_UDP_RECV                   ( SYS_ERROR_UDP_GENERAL - 7 )
#define SYS_ERROR_UDP_DELETE                 ( SYS_ERROR_UDP_GENERAL - 8 )

class CSocketUDP
{
public:
	CSocketUDP();
	~CSocketUDP();

	//服务区端
	int OpenUDPServer(const char* hostIpAddr, unsigned int hostPort);
	int SendUDPServer(int sock, char* buffer, int nLen);
	int RecvUDPServer(char* buffer, int& nRecvLen);
	int GetServerUDPSocket();

	int OpenUDPClient(const char* szServerIp, int nServerPort);
	int SendUDPClient(char *buffer, int bufferLen);
	int GetClientUDPSocket();
	
	int CloseUDP(int newsock);
	char* GetGuestUDPIpAddr();
	int GetGuestUDPPort();
	char* GetUDPErroLog();

	//公共socket函数
	int CreateUDP(int& sock);
	int BindUDPToAddr(int sock, const char* ipAddr, unsigned int port);
	int SetupUDPModel(int socket, int model);
private:
	int        m_listenSock;
	int        m_sendSocket;
	char       m_szGuestIpAddr[32];
	int        m_nGuestPort;
	struct sockaddr_in	m_clientAddr;
	socklen_t	        m_clientAddrlen;
	char       m_szErroLog[512];
	
};

#endif
