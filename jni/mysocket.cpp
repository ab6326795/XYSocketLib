#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include <jni.h>
#include <math.h>
#include "mytcp.h"


/*******************************************************************/
/* 动态库版本号 */
const int SO_VERSION_CODE = 1;
/* 包名 + 类名 */
const char *pClassPathName = "com/pwdgame/jni/IMProtocol";
/* 销毁内存 */
#define JNI_DELETE(ptr) {if(ptr != NULL){delete ptr;ptr = NULL;}}
/*******************************************************************/

static MyTcp* s_pTcpObject;

JNIEXPORT jint JNICALL Init(JNIEnv* env,jclass cls, jstring ip, jint port){
	JNI_DEBUG_LOGCAT("Init");
	if(s_pTcpObject){
		delete s_pTcpObject;
		s_pTcpObject = NULL;
	}
	s_pTcpObject = new MyTcp();

	int ret = -1;
	const char* nativeIP = env->GetStringUTFChars(ip, 0);
	int len = env->GetStringUTFLength(ip);
	//LOGE("ip:%s,port:%d",nativeIP,port);
	if(nativeIP == NULL || len <=0){
		goto Exit;
	}
	ret = s_pTcpObject->Init(nativeIP, port);

	Exit:if(nativeIP != NULL){
		env->ReleaseStringUTFChars(ip, nativeIP);
		nativeIP = NULL;
	}
	return ret;
}

JNIEXPORT jint JNICALL Send(JNIEnv* env, jclass cls,jbyteArray buf,jint offset, jint size){
	if(s_pTcpObject == NULL){
		return -1;
	}
	jint ret = -1;
	jbyte* pBuffer = env->GetByteArrayElements(buf, 0);
	if(NULL == pBuffer){
		return -1;
	}
	ret = s_pTcpObject->Send((char*)pBuffer + offset ,size);

	if(pBuffer !=NULL){
		env->ReleaseByteArrayElements(buf, pBuffer, JNI_ABORT);
		pBuffer = NULL;
	}
	return ret;
}

JNIEXPORT jint JNICALL Recv(JNIEnv* env, jclass cls, jbyteArray buf, jint offset,jint size){
	if(s_pTcpObject == NULL){
		return -1;
	}
	jint ret = -1;
	unsigned char isCopy = 1;
	jbyte* pTempBuffer = new jbyte[size + 2];
	ret = (jint)s_pTcpObject->Recv((char*) pTempBuffer,size);

	if(ret >0 ){
		env->SetByteArrayRegion(buf,offset,ret, pTempBuffer);
	}

	if(pTempBuffer != NULL){
		delete pTempBuffer;
	}
	return ret;
}

JNIEXPORT jint JNICALL Close(JNIEnv *env, jclass cls){
	JNI_DEBUG_LOGCAT("Close");
	if(s_pTcpObject == NULL){
		return -1;
	}
	delete s_pTcpObject;
	s_pTcpObject = NULL;
	return 0;
}

/**
 * 返回SO SDK版本
 */
JNIEXPORT jint JNICALL GetSdkVersion(JNIEnv *env,jclass cls){
	return SO_VERSION_CODE;
}

JNIEXPORT jstring JNICALL GetErrorMessage(JNIEnv * env, jclass cls){
	JNI_DEBUG_LOGCAT("GetErrorMessage");
	if(s_pTcpObject == NULL){
		return env->NewStringUTF("connection is null");
	}
	char* errmsg = s_pTcpObject->GetErrMsg();
	return env->NewStringUTF((const char*)errmsg);
}

JNINativeMethod methods[] = {
		{ "Init", "(Ljava/lang/String;I)I", (void*)Init},
		{ "Close", "()I", (void*)Close},
		{ "Send", "([BII)I", (void*)Send},
		{ "Recv", "([BII)I", (void*)Recv},
		{ "GetSdkVersion", "()I", (void*)GetSdkVersion},
		{ "GetErrorMessage", "()Ljava/lang/String;", (void*)GetErrorMessage}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved){
	JNIEnv* env = NULL;
	if(jvm->GetEnv((void**)&env,JNI_VERSION_1_4) != JNI_OK){
		return JNI_ERR;
	}
	JNI_DEBUG_LOGCAT("find");
	jclass clazz = env->FindClass(pClassPathName);
	JNI_DEBUG_LOGCAT(pClassPathName);
	if(clazz == NULL){
		return JNI_ERR;
	}
	int count = sizeof(methods) / sizeof(methods[0]);
	if(env->RegisterNatives(clazz,methods,count) < 0){
		return JNI_ERR;
	}
	return JNI_VERSION_1_4;

}

#ifdef __cplusplus
}

#endif
