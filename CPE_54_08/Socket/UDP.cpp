#include "UDP.h"

#pragma warning(disable : 4996)

CSocketUDP::CSocketUDP()
{
	m_listenSock = INVALID_SOCKET;
	m_sendSocket = INVALID_SOCKET;
	memset(m_szGuestIpAddr, 0, sizeof(m_szGuestIpAddr));
	m_nGuestPort = 0;
	memset(&m_clientAddr, 0, sizeof(m_clientAddr));
	memset(m_szErroLog, 0, sizeof(m_szErroLog));
}


CSocketUDP::~CSocketUDP()
{

}

/**********************************************************************
*函数名 : OpenUDPServer
*函数功能描述 : 打开绑定的套接口去监听连接请求
*函数参数 : hostPort-监听的服务器端口号（传入）listenSocket-监听的服务器套接口（传出）
*函数返回值 : OK-成功 SYS_ERROR_UDP_CREATE-失败
***********************************************************************/
int CSocketUDP::OpenUDPServer(const char* hostIpAddr, unsigned int hostPort)
{
	//创建流式套接口
	if (OK != CreateUDP(m_listenSock))
	{
		strcpy(m_szErroLog, (char*)"Create a stream socket failed.");
		return SYS_ERROR_UDP_CREATE;
	}

	//设置套接字的模式(非阻塞模式)
	if (OK != SetupUDPModel(m_listenSock, 1))
	{
		strcpy(m_szErroLog, (char*)"Setup mode failure.");
		return SYS_ERROR_UDP_MODEL;
	}

	//绑定套接口到指定的ip地址和端口上
	if (OK != BindUDPToAddr(m_listenSock, hostIpAddr, hostPort))
	{
		strcpy(m_szErroLog, (char*)"Bind the socket to the specified IP address and port failure.");
		return SYS_ERROR_UDP_BIND;
	}
	return OK;
}

/**********************************************************************
*函数名 : OpenUDPClient
*函数功能描述 : 打开绑定的套接口和打开绑定的本地地址
*函数参数 : szServerIp-服务端IP地址（传入）nServerPort-服务端端口号（传入）
*函数返回值 : OK-成功 SYS_ERROR_UDP_GENERAL/SYS_ERROR_UDP_CREATE-失败
***********************************************************************/
int CSocketUDP::OpenUDPClient(const char* szServerIp, int nServerPort)
{
	if ( (szServerIp == NULL) || (nServerPort < 5000) ) 
	{
		return SYS_ERROR_UDP_GENERAL;
	}

	//创建客户端Socket
	int createTime = 0;
	while (m_sendSocket == INVALID_SOCKET)
	{
		m_sendSocket = socket(AF_INET, SOCK_DGRAM, 0);

		if (++createTime > 2)
		{
			return SYS_ERROR_UDP_CREATE;
		}
	}

	//绑定本地地址
	m_clientAddr.sin_family       = AF_INET;
	m_clientAddr.sin_addr.s_addr  = inet_addr(szServerIp);
	m_clientAddr.sin_port         = htons((unsigned short)nServerPort);
	m_clientAddrlen				  = sizeof(struct sockaddr_in);

	return OK;
}



/**********************************************************************
*函数名 : SendUDPClient
*函数功能描述 : 向一个已连接的套接口发送数据
*函数参数 : buffer-发送的数据缓冲区（传入） nLen-发送的数据缓冲区长度（传入）
*函数返回值 : OK-成功 SYS_ERROR_UDP_CREATE/SYS_ERROR_UDP_SEND-失败
***********************************************************************/
int CSocketUDP::SendUDPClient(char *buffer, int bufferLen)
{
	if (m_sendSocket == INVALID_SOCKET)
	{
		return SYS_ERROR_UDP_CREATE;
	}

	int errcode; 

	if( sendto(m_sendSocket,buffer,bufferLen,0,(struct sockaddr*)&m_clientAddr,m_clientAddrlen) == SOCKET_ERROR)//sizeof(clientAddr)
	{
		errcode=(int)errno; 
		if(errcode == EWOULDBLOCK)
		{  
			strcpy(m_szErroLog,(char*)"Sending data to the connection socket failed.");
			return SYS_ERROR_UDP_SEND;
		}  
		else 
		{  
			if(errcode==ETIMEDOUT || errcode==ENETDOWN)  
			{  
				strcpy(m_szErroLog, (char*)"Sending data to the connection socket failed.");
				return SYS_ERROR_UDP_SEND;
			}  
		}  
	}
	return OK;
}

/**********************************************************************
*函数名 : GetClintUDPSocket
*函数功能描述 :得到客户端的套接字
*函数参数 : 
*函数返回值 :
***********************************************************************/
int CSocketUDP::GetClientUDPSocket()
{
	return m_sendSocket;
}


/**********************************************************************
*函数名 : SendUDP
*函数功能描述 :向一个已连接的套接口发送数据
*函数参数 : sock-创建的连接套接口（传入)  buffer-发送的数据缓冲区（传入）
            nLen-发送的数据缓冲区长度（传入）
*函数返回值 : OK-成功 SYS_ERROR_UDP_SEND-失败
*缓冲数据长度建议：局域网内建议长度：1024；因特网中建议长度：512.
***********************************************************************/
int CSocketUDP::SendUDPServer(int sock, char* buffer, int nBufLen)
{
	struct sockaddr_in	clientAddr;
	socklen_t	        addrlen;
	int                 errcode; 

	clientAddr.sin_family       = AF_INET;
	clientAddr.sin_addr.s_addr  = inet_addr(m_szGuestIpAddr);
	clientAddr.sin_port         = htons((unsigned short)m_nGuestPort);
	addrlen					    = sizeof(struct sockaddr_in);

	if( sendto(sock,buffer,(int)strlen(buffer),0,(struct sockaddr*)&clientAddr,addrlen) == SOCKET_ERROR)//sizeof(clientAddr)
	{
		errcode=errno; 
		if(errcode == EWOULDBLOCK)
		{  
			strcpy(m_szErroLog,(char*)"Sending data to the connection socket failed.");
			return SYS_ERROR_UDP_SEND;
		}  
		else 
		{  
			if(errcode==ETIMEDOUT || errcode==ENETDOWN)  
			{  
				strcpy(m_szErroLog, (char*)"Sending data to the connection socket failed.");
				return SYS_ERROR_UDP_SEND;
			}  
		}  
	}

	return OK;
}

/**********************************************************************
*函数名 : RecvUDP
*函数功能描述 :向一个已连接的套接口接收数据
*函数参数 : sock-创建的连接套接口（传入)  buffer-接收数据的缓冲区（传入）
            nLen-接收的数据的缓冲区长度（传入） nRecvLen-接收数据的长度（传出）
*函数返回值 : OK-成功 SYS_ERROR_UDP_RECV-失败
***********************************************************************/
int CSocketUDP::RecvUDPServer(char* buffer, int& nRecvLen)
{
	struct sockaddr_in	clientAddr;
	socklen_t	        addrlen;
	int					errcode;

	addrlen  = sizeof(struct sockaddr_in);
	m_nGuestPort = 0;
	memset(m_szGuestIpAddr, 0, sizeof(m_szGuestIpAddr));

	memset(buffer, 0, strlen(buffer));

	if(m_listenSock==INVALID_SOCKET)
	{
		strcpy(m_szErroLog, (char*)"The bad socket.");
	}
	if((nRecvLen = recvfrom(m_listenSock,buffer,256,0,(struct sockaddr*)&clientAddr,&addrlen)) == SOCKET_ERROR)  
	{  
		errcode=(int)errno;  
		if(errcode == EWOULDBLOCK)
		{  
			strcpy(m_szErroLog, (char*)"Recving data from the connection socket failed.");
		}  
		else 
		{  
			if(errcode==ETIMEDOUT || errcode==ENETDOWN)  
			{    
				strcpy(m_szErroLog, (char*)"Recving data from the connection socket failed.");
			}  
		} 
		return SYS_ERROR_UDP_RECV;
	}
	else
	{
		strcpy(m_szGuestIpAddr, inet_ntoa(clientAddr.sin_addr));
		m_nGuestPort = ntohs(clientAddr.sin_port);
	}

	return OK;
}

/**********************************************************************
*函数名 : CloseUDP
*函数功能描述 :关闭套接口
*函数参数 : newsock-创建的连接套接口（传入）
*函数返回值 : OK-成功 SYS_ERROR_UDP_CLOSE-失败
***********************************************************************/
int CSocketUDP::CloseUDP(int newsock)
{
	if (SOCKET_ERROR == close(newsock))
	{
		strcpy(m_szErroLog, (char*)"Closing socket failed.");
		return SYS_ERROR_UDP_DELETE;
	}
	return OK;
}

/**********************************************************************
*函数名 : GetServerUDPSocket
*函数功能描述 :得到服务端的套接字
*函数参数 : 
*函数返回值 :
***********************************************************************/
int CSocketUDP::GetServerUDPSocket()
{
	return m_listenSock;
}
/**********************************************************************
*函数名 : GetGuestUDPIpAddr
*函数功能描述 :得到新连接客户端的IP地址
*函数参数 : 
*函数返回值 :
***********************************************************************/
char* CSocketUDP::GetGuestUDPIpAddr()
{
	return m_szGuestIpAddr;
}

/**********************************************************************
*函数名 : GetGuestUDPPort
*函数功能描述 :得到新连接客户端的端口号
*函数参数 : 
*函数返回值 :
***********************************************************************/
int CSocketUDP::GetGuestUDPPort()
{
	return m_nGuestPort;
}

/**********************************************************************
*函数名 : GetUDPErroLog
*函数功能描述 :得到系统错误日志
*函数参数 : 
*函数返回值 :
***********************************************************************/
char* CSocketUDP::GetUDPErroLog()
{
	return m_szErroLog;
}

/**********************************************************************
*函数名 : BindUDPToAddr
*函数功能描述 : 绑定socket到本机的ip地址和port上
*函数参数 : sock-创建的监听套接口（传入）  ipAddr-监听的ip地址（传入）
            port-监听的端口号（传入）
*函数返回值 : OK-成功 SYS_ERROR_UDP_BIND-失败
***********************************************************************/
int CSocketUDP::BindUDPToAddr(int sock, const char* ipAddr, unsigned int port)
{
	struct sockaddr_in  localAddr;
	
	localAddr.sin_family        = AF_INET;
	localAddr.sin_addr.s_addr = inet_addr(ipAddr);
	localAddr.sin_port          = htons((unsigned short)port);

	if (SOCKET_ERROR == bind(sock, (struct sockaddr *)&localAddr, sizeof(localAddr)))
	{
		return SYS_ERROR_UDP_BIND;
	}

	return OK;
}

/**********************************************************************
*函数名 : CreateUDP
*函数功能描述 : 创建一个流式套接口
*函数参数 : sock-创建的套接口（传出）
*函数返回值 : OK-成功 SYS_ERROR_UDP_CREATE-失败
***********************************************************************/
int CSocketUDP::CreateUDP(int& sock)
{
	//创建socket
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //创建UDP socket
	if (INVALID_SOCKET == sock)
	{
		printf("invalid socket");
		return SYS_ERROR_UDP_CREATE;
	}
	return OK ;
}
/**********************************************************************
*函数名 : SetupUDPModel
*函数功能描述 :用于设置套接口的相关模式
*函数参数 : socket-创建的套接口
            model- 0:为阻塞模式 1:为非阻塞模式
*函数返回值 : OK-成功 SYS_ERROR_UDP_MODEL-失败
***********************************************************************/
int CSocketUDP::SetupUDPModel(int socket, int model)
{
	if (SOCKET_ERROR == ioctl(socket, FIONBIO, (unsigned long*)&model))
	{
		return SYS_ERROR_UDP_MODEL;
	}
	return OK;
}
