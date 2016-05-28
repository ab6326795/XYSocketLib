package com.pwdgame.jni;

public class IMProtocol {

	static{
		System.loadLibrary("XYSocketLib");

	}
	
	public native static int SocketClient_Create(String ip, int port);	
	public native static int SocketClient_Send(byte[] buf,int offset,int size);
	public native static int SocketClient_Recv(byte[] buf,int offset,int size);
	public native static String GetClientErrMsg();
	public native static int SocketClient_Close();
	
	public native static int SocketServer_Create(String ip, int port, int packlog);
	/**
	 * 
	 * @return 返回单个连接的socket ID
	 */
	public native static int SocketServer_Accept();	
	public native static int SocketServer_Send(int socketId, byte[] buf,int offset,int size);
	public native static int SocketServer_Recv(int socketId, byte[] buf,int offset,int size);
	public native static int SocketServer_Close();
	public native static String GetServerErrMsg();
	
	public native static int GetSdkVersion();
	
}
