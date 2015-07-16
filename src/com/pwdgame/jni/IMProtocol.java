package com.pwdgame.jni;

public class IMProtocol {

	static{
		System.loadLibrary("XYSocketLib");
	}
	
	public native static int Init(String ip, int port);
	public native static int Close();
	public native static int Send(byte[] buf,int offset,int size);
	public native static int Recv(byte[] buf,int offset,int size);
	
	public native static int GetSdkVersion();
	public native static String GetErrorMessage();
}
