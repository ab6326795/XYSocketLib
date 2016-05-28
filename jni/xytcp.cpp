#include <stdafx.h>
#include "android_common.h"
#ifndef WIN32

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#define closesocket close
#endif

#include "xytcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum{
	ERRCODE_OK = 0,

	ERRCODE_SEND,   // - 998

};

XYTcp::XYTcp(){
	m_server_listen_id = -1;
	m_server_socket_id = -1;
	m_client_socket_id = -1;
	memset(m_server_errMsg, 0, sizeof(m_server_errMsg));
	memset(m_client_errMsg, 0, sizeof(m_client_errMsg));
}

XYTcp::~XYTcp(){
	SocketServer_Close();
	SocketClient_Close();
}

int XYTcp::SocketServer_Create(const char* szIP, int port, int backlog){
	if(NULL == szIP){
		return -1;
	}
	if(strlen(szIP) == 0){
		return -1;
	}
	SocketServer_Close();

	m_server_listen_id = CreateServerTCPScoket(szIP, port, backlog);
	if(m_server_listen_id < 0){
		//sprintf(m_server_errMsg, "create socket server %s:%d fail", szIP, port);
		return -1;
	}
	return 0;
}

int XYTcp::SocketServer_Accept(char **remoteAddr){
	if(m_server_listen_id < 0){
		sprintf(m_server_errMsg, "socket_server_id is invalid ! please SocketServer_Create first !");
		return -1;
	}

	int addr_len = sizeof(m_remote_addr_in);
	memset((void*)&m_remote_addr_in, 0, addr_len);
	LOGD("SocketServer_Accept >>>> 等待连接中...");
    int nSocket = accept(m_server_listen_id, (struct sockaddr*)&m_remote_addr_in, (socklen_t*)&addr_len);
    if(nSocket >= 0 && remoteAddr != NULL){
    	(*remoteAddr) = inet_ntoa(m_remote_addr_in.sin_addr);
    }
    return nSocket;
}

int XYTcp::SocketServer_Send(int socket_id, char* pkg, int len){
	if(socket_id < 0){
		if(socket_id == m_client_socket_id){
			sprintf(m_server_errMsg, "please SocketClient_Create first !");
		}else{
		    sprintf(m_server_errMsg, "please SocketServer_Create first !");
		}
		return -1;
	}

	int nSendLength = 0;
	int nSendResult = 0;
	while(nSendLength < len){
		nSendResult = send(socket_id, pkg + nSendLength, len - nSendLength, 0);
		if(nSendResult > 0){
			nSendLength += nSendResult;
		}else{
			LOGD("ERRCODE_SEND !!! send data len:%d, already:%d,count:%d", nSendLength, nSendResult, len);
			return ERRCODE_SEND;
		}
	}

	return nSendLength;
}
int XYTcp::SocketServer_Recv(int socket_id, char* pkg, int len){
	if(socket_id <= 0){
		return -1;
	}
	return recv(socket_id, pkg, len, 0);
}

int XYTcp::CreateServerTCPScoket(const char* szIP,unsigned short ushPort, int backlog){
	int nSocketID = 0;
	struct sockaddr_in server_addr;
	nSocketID = socket(AF_INET, SOCK_STREAM, 0);
	if(nSocketID == -1){
		sprintf(m_server_errMsg, "create socket fail !");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(szIP);
	server_addr.sin_port = htons(ushPort);

	int on = 1;
	setsockopt(nSocketID, SOL_SOCKET, 0x1022, (void *)&on, sizeof(on));

	//绑定IP和端口
	if(bind(nSocketID, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		sprintf(m_server_errMsg, "socket server bind %s:%d fail", szIP, server_addr.sin_port);
		close(nSocketID);
		return -1;
	}
	//开始监听
	if(listen(nSocketID, backlog) < 0){
		sprintf(m_server_errMsg, "socket server listen %s:%d fail", szIP, ushPort);
		close(nSocketID);
		return -1;
	}

	return nSocketID;
}
void XYTcp::SocketServer_Close(){
	if(m_server_socket_id > 0){
		close(m_server_socket_id);
		m_server_socket_id = -1;
	}
	if(m_server_listen_id > 0){
		close(m_server_listen_id);
		m_server_listen_id = -1;
	}
}
/*************************************************************/

int XYTcp::SocketClient_Create(const char* szIP, int port){
	if(NULL == szIP){
		return -1;
	}
	if(strlen(szIP) == 0){
		return -1;
	}

	SocketClient_Close();

	m_client_socket_id = CreateClientTCPScoket(szIP,port);
	if(m_client_socket_id < 0){
		sprintf(m_client_errMsg, "connect to server %s:%d fail", szIP, port);
		return -1;
	}
	return m_client_socket_id;
}

int XYTcp::SocketClient_Send(char* pkg, int len){
	return SocketServer_Send(m_client_socket_id, pkg, len);
}

int XYTcp::SocketClient_Recv(char* pkg, int len){
	return SocketServer_Recv(m_client_socket_id, pkg, len);
}

void XYTcp::SocketClient_Close(){
	if(m_client_socket_id > 0){
		close(m_client_socket_id);
		m_client_socket_id = -1;
	}
}
int XYTcp::CreateClientTCPScoket(const char* szIP,unsigned short ushPort){
	int nSocketID = 0;
	struct sockaddr_in server_addr;
	nSocketID = socket(AF_INET, SOCK_STREAM, 0);
	if(nSocketID == -1){
		sprintf(m_server_errMsg, "create socket %s:%d fail", szIP, ushPort);
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(szIP);
	server_addr.sin_port =htons(ushPort);

	int on = 1;
	setsockopt(nSocketID, SOL_SOCKET, 0x1022, (void *)&on, sizeof(on));
	int nConnectResult = connect(nSocketID, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(nConnectResult < 0){
		close(nSocketID);
		return -1;
	}
	return nSocketID;
}

/*************************公共调用******************************/

