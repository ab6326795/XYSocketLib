#include <string.h>
#include <jni.h>
#include <math.h>
#include "xytcp.h"
#include "android_common.h"

/*******************************************************************/
/* 动态库版本号 */
const int SO_VERSION_CODE = 2;
/* 包名 + 类名 */
const char *pClassPathName = "com/pwdgame/jni/IMProtocol";
/* 销毁内存 */
#define JNI_DELETE(ptr) {if(ptr != NULL){delete ptr;ptr = NULL;}}
/*******************************************************************/

static XYTcp* s_pClientTcp;
static XYTcp* s_pServerTcp;

jint SocketClient_Create(JNIEnv* env,jclass cls, jstring ip, jint port){
	if(s_pClientTcp){
		delete s_pClientTcp;
		s_pClientTcp = NULL;
	}
	s_pClientTcp = new XYTcp();

	int ret = -1;
	const char* nativeIP = env->GetStringUTFChars(ip, 0);
	int len = env->GetStringUTFLength(ip);
	LOGD("SocketClient_Create ip:%s,port:%d",nativeIP,port);
	if(nativeIP == NULL || len <=0){
		goto Exit;
	}
	ret = s_pClientTcp->SocketClient_Create(nativeIP, port);

	Exit:if(nativeIP != NULL){
		env->ReleaseStringUTFChars(ip, nativeIP);
		nativeIP = NULL;
	}
	return ret;
}

jint SocketClient_Send(JNIEnv* env, jclass cls,jbyteArray buf,jint offset, jint size){
	if(s_pClientTcp == NULL){
		return -1;
	}
	jint ret = -1;
	jbyte* pBuffer = env->GetByteArrayElements(buf, 0);
	if(NULL == pBuffer){
		return -1;
	}
	ret = s_pClientTcp->SocketClient_Send((char*)pBuffer + offset ,size);

	if(pBuffer !=NULL){
		env->ReleaseByteArrayElements(buf, pBuffer, JNI_ABORT);
		pBuffer = NULL;
	}
	return ret;
}

jint SocketClient_Recv(JNIEnv* env, jclass cls, jbyteArray buf, jint offset,jint size){
	if(s_pClientTcp == NULL){
		return -1;
	}
	jint ret = -1;
	unsigned char isCopy = 1;
	jbyte* pTempBuffer = new jbyte[size + 2];
	ret = (jint)s_pClientTcp->SocketClient_Recv((char*) pTempBuffer,size);

	if(ret >0 ){
		env->SetByteArrayRegion(buf,offset,ret, pTempBuffer);
	}

	if(pTempBuffer != NULL){
		delete pTempBuffer;
	}
	return ret;
}

jint SocketClient_Close(JNIEnv *env, jclass cls){
	if(s_pClientTcp == NULL){
		return -1;
	}
	delete s_pClientTcp;
	s_pClientTcp = NULL;
	return 0;
}


jstring GetClientErrMsg(JNIEnv * env, jclass cls){
	if(s_pClientTcp == NULL){
		return env->NewStringUTF("connection is null");
	}
	char* errmsg = s_pClientTcp->GetClientErrMsg();
	return env->NewStringUTF((const char*)errmsg);
}

/*********************************************/

jint SocketServer_Create(JNIEnv* env,jclass cls, jstring ip, jint port, jint packlog){
	if(s_pServerTcp){
		delete s_pServerTcp;
		s_pServerTcp = NULL;
	}
	s_pServerTcp = new XYTcp();

	int ret = -1;
	const char* nativeIP = env->GetStringUTFChars(ip, 0);
	int len = env->GetStringUTFLength(ip);
	LOGD("SocketServer_Create ip:%s,port:%d",nativeIP,port);
	if(nativeIP == NULL || len <=0){
		goto Exit;
	}
	ret = s_pServerTcp->SocketServer_Create(nativeIP, port, packlog);

	Exit:if(nativeIP != NULL){
		env->ReleaseStringUTFChars(ip, nativeIP);
		nativeIP = NULL;
	}
	return ret;
}
jint SocketServer_Accept(JNIEnv* env,jclass cls){
	if(s_pServerTcp == NULL){
		return -1;
	}
	char* buf = (char*)malloc(sizeof(char) * 128);
	int socket_id = s_pServerTcp->SocketServer_Accept((char**)&buf);
    if(socket_id > 0){
    	LOGE("SocketServer_Accept, ip:%s", buf);
    }
    free(buf);
	return socket_id;
}

jint SocketServer_Send(JNIEnv* env, jclass cls,jint socket_id,jbyteArray buf,jint offset, jint size){
	if(s_pServerTcp == NULL){
		return -1;
	}
	jint ret = -1;
	jbyte* pBuffer = env->GetByteArrayElements(buf, 0);
	if(NULL == pBuffer){
		return -1;
	}
	ret = s_pServerTcp->SocketServer_Send(socket_id, (char*)pBuffer + offset ,size);

	if(pBuffer !=NULL){
		env->ReleaseByteArrayElements(buf, pBuffer, JNI_ABORT);
		pBuffer = NULL;
	}
	return ret;
}

jint SocketServer_Recv(JNIEnv* env, jclass cls,jint socket_id, jbyteArray buf, jint offset,jint size){
	if(s_pServerTcp == NULL){
		return -1;
	}
	jint ret = -1;
	unsigned char isCopy = 1;
	jbyte* pTempBuffer = new jbyte[size + 2];
	ret = (jint)s_pServerTcp->SocketServer_Recv(socket_id, (char*) pTempBuffer,size);

	if(ret >0 ){
		env->SetByteArrayRegion(buf,offset,ret, pTempBuffer);
	}

	if(pTempBuffer != NULL){
		delete pTempBuffer;
	}
	return ret;
}

jint SocketServer_Close(JNIEnv *env, jclass cls){
	if(s_pServerTcp == NULL){
		return -1;
	}
	delete s_pServerTcp;
	s_pServerTcp = NULL;
	return 0;
}


jstring GetServerErrMsg(JNIEnv * env, jclass cls){
	if(s_pServerTcp == NULL){
		return env->NewStringUTF("connection is null");
	}
	char* errmsg = s_pServerTcp->GetServerErrMsg();
	return env->NewStringUTF((const char*)errmsg);
}


/**
 * 返回SO SDK版本
 */
jint GetSdkVersion(JNIEnv *env,jclass cls){
	return SO_VERSION_CODE;
}

JNINativeMethod methods[] = {
		{ "SocketClient_Create", "(Ljava/lang/String;I)I", (void*)SocketClient_Create},
		{ "SocketClient_Close", "()I", (void*)SocketClient_Close},
		{ "SocketClient_Send", "([BII)I", (void*)SocketClient_Send},
		{ "SocketClient_Recv", "([BII)I", (void*)SocketClient_Recv},
		{ "GetClientErrMsg", "()Ljava/lang/String;", (void*)GetClientErrMsg},

		{ "SocketServer_Create", "(Ljava/lang/String;II)I", (void*)SocketServer_Create},
		{ "SocketServer_Accept", "()I", (void*)SocketServer_Accept},
		{ "SocketServer_Close", "()I", (void*)SocketServer_Close},
		{ "SocketServer_Send", "(I[BII)I", (void*)SocketServer_Send},
		{ "SocketServer_Recv", "(I[BII)I", (void*)SocketServer_Recv},
		{ "GetServerErrMsg", "()Ljava/lang/String;", (void*)GetServerErrMsg},

		{ "GetSdkVersion", "()I", (void*)GetSdkVersion},

};

extern "C" __attribute__ ((visibility ("default"))) jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved){
	JNIEnv* env = NULL;
	if(jvm->GetEnv((void**)&env,JNI_VERSION_1_4) != JNI_OK){
		return JNI_ERR;
	}

	jclass clazz = env->FindClass(pClassPathName);
	LOGD("JNI_OnLoad %s", pClassPathName);
	if(clazz == NULL){
		return JNI_ERR;
	}
	int count = sizeof(methods) / sizeof(methods[0]);
	if(env->RegisterNatives(clazz,methods,count) < 0){
		return JNI_ERR;
	}
	return JNI_VERSION_1_4;

}

