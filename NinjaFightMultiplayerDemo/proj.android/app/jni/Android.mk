LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/HelloWorldScene.cpp \
                   ../../../Classes/Player.cpp \
                   ../../../Classes/StartGameLayer.cpp \
                   ../../../AppWarpX/HMAC_SHA1.cpp \
                   ../../../AppWarpX/SHA1.cpp \
                   ../../../AppWarpX/appwarp.cpp \
                   ../../../AppWarpX/appwarp_extended.cpp \
                   ../../../AppWarpX/base64.cpp \
                   ../../../AppWarpX/requests.cpp \
                   ../../../AppWarpX/socket.cpp \
                   ../../../AppWarpX/udpsocket.cpp \
                   ../../../AppWarpX/urlencode.cpp \
                   ../../../AppWarpX/utilities.cpp \
                   ../../../AppWarpX/cJSON.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../AppWarpX
#                    $(LOCAL_PATH)/../../../cocos2d/external/curl/include/android

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += cocos_curl_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d/external/curl/prebuilt)
$(call import-module, cocos)
$(call import-module, ./android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
