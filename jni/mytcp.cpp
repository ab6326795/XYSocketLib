#include <stdafx.h>

#ifndef WIN32

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#define closesocket close
#endif

#include "mytcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum{
	ERRCODE_OK = 0,
	ERRCODE_SENDBUFF = -1001,
	ERRCODE_CONN, // - 1000
	ERRCODE_INPUT,  // - 999
	ERRCODE_SEND,   // - 998
	ERRCODE_RECV,   // - 997
	ERRCODE_CLOSE,  // -996
	ERRCODE_RESPHTTPHEAD, // -995
	ERRCODE_TIMEOUT, // -994
	ERRCODE_INVALIDSOCK, // -993
	ERRCODE_INVALIDPKG, // -992
};

MyTcp::MyTcp(){
	m_socket = -1;
}

MyTcp::~MyTcp(){
	if(m_socket > 0){
		close(m_socket);
		m_socket = -1;
	}
}

int MyTcp::Init(const char* szIP, int port){
	if(NULL == szIP){
		return -1;
	}
	if(strlen(szIP) == 0){
		return -1;
	}

	if(m_socket > 0){
		close(m_socket);
		m_socket = -1;
	}

	m_socket = CreateClientTCPScoket(szIP,port);
	if(m_socket < 0){
		sprintf(m_errMsg, "connect to server %s:%d fail", szIP, port);
		return ERRCODE_INVALIDSOCK;
	}
	return 0;
}

int MyTcp::Send(char* pkg, int len){
	if(m_socket < 0){
		sprintf(m_errMsg, "please init first !");
		return ERRCODE_INVALIDSOCK;
	}

	int nSendLength = 0;
	int nSendResult = 0;
	while(nSendLength < len){
		nSendResult = send(m_socket,pkg + nSendLength,len - nSendLength, 0);
		if(nSendLength > 0){
			nSendLength += nSendResult;
		}else{
			return ERRCODE_SEND;
		}
	}

	return 0;
}

int MyTcp::Recv(char* pkg, int len){
	if(m_socket <= 0){
		return -1;
	}
	return recv(m_socket, pkg, len, 0);
}

int MyTcp::CreateClientTCPScoket(const char* szIP,unsigned short ushPort){
	int nSocketID = 0;
	struct sockaddr_in server_addr;
	nSocketID = socket(AF_INET, SOCK_STREAM, 0);
	if(nSocketID == -1){
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
