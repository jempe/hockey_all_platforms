LOCAL_PATH := $(call my-dir)

PATH_TO_LIBS = libs
  
include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_CFLAGS += -DCOCOS2D_DEBUG=1

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/MenuSprite.cpp \
                   ../../Classes/SquareButton.cpp \
                   ../../Classes/VectorSprite.cpp \
                   ../../Classes/MenuScene.cpp \
                   ../../Classes/LevelsScene.cpp \
                   ../../Classes/HockeyScene.cpp \
                   ../../Classes/CCLocalizedString.cpp \
                   ../../Classes/RevMob.cpp \
                   ../../Classes/RevMobAds_Android.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes            

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
