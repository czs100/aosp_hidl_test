LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE := bidl_hello_test
LOCAL_SRC_FILES := \
    HidlTest.cpp \

LOCAL_SHARED_LIBRARIES := \
   liblog \
   libhidlbase \
   libutils \
   android.hardware.helloworld@1.0 \

include $(BUILD_EXECUTABLE)