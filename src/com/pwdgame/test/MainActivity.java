package com.pwdgame.test;

import java.util.logging.Logger;

import android.R.bool;
import android.R.integer;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

import com.pwdgame.jni.IMProtocol;

public class MainActivity extends Activity{

	private static final String TAG = "ajm";
	private static boolean isServerRun = true;
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		//随便设置一个Layout，我们这里只看Log
		setContentView(android.R.layout.activity_list_item);	
		
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					int ret = IMProtocol.SocketServer_Create("127.0.0.1", 666666, 5);					
					if(ret < 0){
					    Log.e("ajm", "server ->SocketServer_Create :" + ret + ",msg:" + IMProtocol.GetServerErrMsg());
					}else{
						showToast("server ->SocketServer_Create success !");
						byte[] buf = new byte[256];
						
						while(isServerRun){
							int socketId = IMProtocol.SocketServer_Accept();
							if(socketId > 0){
								
								int recv_len = IMProtocol.SocketServer_Recv(socketId, buf, 0, buf.length);
								if(recv_len > 0){
									Log.e("ajm", "server ->收到消息:" + new String(buf, 0, recv_len, "utf-8"));
								}
							}else{
								Log.e("ajm", "server ->error:" + IMProtocol.GetServerErrMsg());
								break;
							}
						}
					}
								
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
		
	}
	boolean b = true;
	private void test(){
		if(b){
			if(IMProtocol.SocketClient_Create("127.0.0.1", 666666) > 0){
				byte[] sendBytes = "总部总部，呼叫总部！".getBytes();
				int sendLen = IMProtocol.SocketClient_Send(sendBytes, 0, sendBytes.length); 
				if(sendLen > 0){
					showToast("client -> 消息发送成功！len =" +sendLen);
				}else{
					showToast("client ->消息发送失败,len= " +sendLen +","+ IMProtocol.GetClientErrMsg());
				}
				
				if(IMProtocol.SocketClient_Close() > 0){
					showToast("client ->连接已经成功关闭");
				}else{
					showToast("client ->连接已经关闭失败");
				}
			}else{
				showToast("client ->连接失败！");
			}
			
			b = false;
		}else{
			b = true;
			IMProtocol.SocketServer_Close();
		}
	}
	
	private void showToast(String str){
		//打印日志和TOAST
		Log.d(TAG, str);
		//Toast.makeText(this, str, Toast.LENGTH_SHORT).show();		
	}

	
	@Override
	public boolean onTouchEvent(MotionEvent event){
		//每次点击都开始测试
		if(event.getAction() == MotionEvent.ACTION_DOWN){			
			test();
			return true;
		}
		return super.onTouchEvent(event);
	}
}
