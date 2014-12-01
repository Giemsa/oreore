LOCAL_PATH := $(call my-dir)
COCOS_PATH = $(LOCAL_PATH)/../../example/cocos2d

include $(CLEAR_VARS)

LOCAL_MODULE := oreore_static
LOCAL_MODULE_FILENAME := liboreore

FILE_LIST := $(wildcard $(LOCAL_PATH)/../oreore/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../oreore/cocos2d/**/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/core \
                    $(LOCAL_PATH)/cocos2dx \
                    $(COCOS_PATH) \
                    $(COCOS_PATH)/cocos \
                    $(COCOS_PATH)/cocos/2d \
                    $(COCOS_PATH)/cocos/2d/platform \
                    $(COCOS_PATH)/cocos/base \
                    $(COCOS_PATH)/cocos/math/kazmath \
                    $(COCOS_PATH)/cocos/audio/incude \
                    $(COCOS_PATH)/extensions \
                    $(COCOS_PATH)/cocos/2d/platform/android

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../oreore \
                           $(LOCAL_PATH)/../oreore/cocos2d \
                           $(LOCAL_PATH)/../oreore/cocos2d/actions \
                           $(LOCAL_PATH)/../oreore/cocos2d/nodes \
                           $(LOCAL_PATH)/../oreore/cocos2d/utils \
                           $(LOCAL_PATH)/../oreore/libs/msgpack-c/include

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_extension_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,extensions)
