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
	 * @return >0 返回单个连接的socket ID，否则说明服务器可能已经关闭。
	 */
	public native static int SocketServer_Accept();	
	
	/**
	 * 服务器端 发送数据
	 * @param socketId
	 * @param buf
	 * @param offset
	 * @param size
	 * @return 返回发送出去的字节数
	 */
	public native static int SocketServer_Send(int socketId, byte[] buf,int offset,int size);
	
	/**
	 * 服务器端 接收数据
	 * @param socketId
	 * @param buf
	 * @param offset
	 * @param size
	 * @return 返回接收到的字节数
	 */
	public native static int SocketServer_Recv(int socketId, byte[] buf,int offset,int size);
	
	/**
	 * 关闭服务器端
	 * @return
	 */
	public native static int SocketServer_Close();
	
	/**
	 * 获取服务器端 错误消息
	 * @return
	 */
	public native static String GetServerErrMsg();
	
	public native static int GetSdkVersion();
	
}
