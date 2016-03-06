LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
#OPENCV_LIB_TYPE:=SHARED
include ../../sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := optflow_jni.cpp eyelike/findEyeCorner.cpp eyelike/findEyeCenter.cpp eyelike/helpers.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_CFLAGS += -std=c++11

LOCAL_MODULE     := eyemon

include $(BUILD_SHARED_LIBRARY)
