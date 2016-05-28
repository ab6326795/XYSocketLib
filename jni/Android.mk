LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    :=XYSocketLib
LOCAL_SRC_FILES := mysocket.cpp \
                   xytcp.cpp

LOCAL_LDLIBS :=  -llog
LOCAL_PRELINK_MODULE := false
APP_ABI := armeabi armeabi-v7a x86 mips 
include $(BUILD_SHARED_LIBRARY)
