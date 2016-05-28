#ifndef XYTCP_H
#define XYTCP_H

#include <netinet/in.h>

#define MAX_BODY_LEN 10960
/* 数据强制连续排列不对齐 */
#pragma pack(1)

class XYTcp{
public:
	XYTcp();
	~XYTcp();
public:
	/********* socket server api *************/
	int SocketServer_Create(const char* ip, int port, int backlog);
	int SocketServer_Accept(char **remoteAddr);
	char* GetServerErrMsg(){
		return (char*)m_server_errMsg;
	}
	int SocketServer_Send(int socket_id, char* pkg, int len);
	int SocketServer_Recv(int socket_id, char* pkg, int len);
	void SocketServer_Close();

	/********* socket client api*************/
	int SocketClient_Create(const char* ip, int port);
	int SocketClient_Send(char* pkg, int len);
	int SocketClient_Recv(char* pkg, int len);
	char* GetClientErrMsg(){
		return (char*)m_client_errMsg;
	}
	void SocketClient_Close();
	/**********************/

private:
	int CreateClientTCPScoket(const char* szIP,unsigned short ushPort);
	int CreateServerTCPScoket(const char* szIP,unsigned short ushPort, int backlog);
private:
	int m_server_listen_id;
	int m_server_socket_id;
	sockaddr_in m_remote_addr_in;

	int m_client_socket_id;
	char m_server_errMsg[1024];
	char m_client_errMsg[1024];
};




#endif
