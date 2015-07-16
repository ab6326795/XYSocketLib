package com.pwdgame.test;

import java.util.logging.Logger;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

import com.pwdgame.jni.IMProtocol;

public class MainActivity extends Activity{

	private static final String TAG = MainActivity.class.getSimpleName();
	
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		//随便设置一个Layout，我们这里只看Log
		setContentView(android.R.layout.activity_list_item);	
	}
	
	private void test(){
		if(IMProtocol.Init("10.0.0.98", 11222) > 0){
			byte[] sendBytes = "总部总部，呼叫总部！".getBytes();
			if(IMProtocol.Send(sendBytes, 0, sendBytes.length) > 0){
				showToast("消息发送成功！");
			}else{
				showToast("消息发送失败！" + IMProtocol.GetErrorMessage());
			}
			
			if(IMProtocol.Close() > 0){
				showToast("连接已经成功关闭");
			}else{
				showToast("连接已经关闭失败");
			}
		}else{
			showToast("连接失败！");
		}
	}
	
	private void showToast(String str){
		//打印日志和TOAST
		Log.e(TAG, str);
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();		
	}

	
	@Override
	public boolean onTouchEvent(MotionEvent event){
		//每次点击都开始测试
		if(event.getAction() == MotionEvent.ACTION_DOWN){
			showToast("开始测试。。。");
			test();
			return true;
		}
		return super.onTouchEvent(event);
	}
}
