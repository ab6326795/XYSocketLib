#ifndef  ANDROID_COMMON_H
#define ANDROID_COMMON_H

#include <jni.h>
#include <android/Log.h>
#include <stdbool.h>
#include <stddef.h>


#define LOG_TAG "ajm"
/**是否打印DEBUG信息*/
#define DEBUG 1

/***************** 类型定义 ************************/
typedef unsigned short  UShort;
typedef unsigned int    UInt;
typedef int             Long;
typedef unsigned int    ULong;
typedef unsigned char   UChar;
typedef unsigned long long  ULongLong;

/***************** Log宏定义 ************************/
#define LOGV(...) {if(DEBUG){__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__);}}
#define LOGD(...) {if(DEBUG){__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);}}
#define LOGI(...) {if(DEBUG){__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}}
#define LOGW(...) {if(DEBUG){__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);}}
#define LOGE(...) {if(DEBUG){__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}}

#define LOGE_STRING(str) LOGE("%s",str)
/***************** 常用宏定义 ************************/
/* 销毁内存 */
#define JNI_DELETE(ptr) {if(ptr != NULL){delete ptr;ptr = NULL;}}

// TODO: reference additional headers your program requires here

#include <linux/compiler.h>
#undef NULL
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif



#define in
#define out


#endif
