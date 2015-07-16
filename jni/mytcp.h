#ifndef TCP_H
#define TCP_H

#include <android/Log.h>
#define LOG_TAG "IMProtocol"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define MAX_BODY_LEN 10960
/* 数据强制连续排列不对齐 */
#pragma pack(1)

class MyTcp{
public:
	MyTcp();
	~MyTcp();
public:
	int Init(const char* ip,int port);
	char* GetErrMsg(){
		return (char*)m_errMsg;
	}

	int Send(char* pkg, int len);
	int Recv(char* pkg, int len);
private:
	int CreateClientTCPScoket(const char* szIP,unsigned short ushPort);
private:
	int m_socket;
	char m_errMsg[1024];
};


#ifndef TCP_H_DEBUG
#define TCP_H_DEBUG
/* Test Flags */
#define JNI_DEBUG_JAVA_LOGCAT
void JNI_DEBUG_LOGCAT(const char* pMessage){
#ifdef JNI_DEBUG_JAVA_LOGCAT
	LOGI("%s", pMessage);
#endif
}
#endif



#endif
