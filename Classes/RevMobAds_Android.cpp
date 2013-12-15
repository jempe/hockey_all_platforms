#include "RevMobAds_Android.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

#ifndef CC_TARGET_PLATFORM
#error "CC_TARGET_PLATFORM should be defined!!!"
#endif

namespace revmob {

const char *RevMobWrapperClass = "com/revmob/cocos2dx/RevMobWrapper";

static void callStaticVoidMethod(const char *name, const char *signature, ...)
{
	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
											RevMobWrapperClass,
			                                name,
			                                signature) ) {
		va_list args;
		va_start(args, signature);
		methodInfo.env->CallStaticVoidMethodV(methodInfo.classID, methodInfo.methodID, args);
		va_end(args);

	}
}

static int callStaticIntMethod(const char *name, const char *signature, ...)
{
	cocos2d::JniMethodInfo methodInfo;

	jint ret = 0;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
											RevMobWrapperClass,
			                                name,
			                                signature) ) {
		va_list args;
		va_start(args, signature);
		ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, args);
		va_end(args);
	}
	return ret;
}

void RevMobAds_Android::StartSession(const char *pAppID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	cocos2d::JniMethodInfo methodInfo;

	if ( cocos2d::JniHelper::getStaticMethodInfo( methodInfo,
												RevMobWrapperClass,
												"startSession",
												"(Ljava/lang/String;)V") ) {
		jstring stringArg = methodInfo.env->NewStringUTF(pAppID);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
	}
#endif
}


void RevMobAds_Android::PrintEnvironmentInformation()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callStaticVoidMethod("printEnvironmentInformation","()V");
#endif
}

void RevMobAds_Android::SetTestingMode(TestingMode pTestingMode)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    switch (pTestingMode) {
        case kTestingModeWithAds:
        	callStaticVoidMethod("setTestingMode","(I)V",1);
            break;
        case kTestingModeWithoutAds:
        	callStaticVoidMethod("setTestingMode","(I)V",2);
            break;
        case kTestingModeDisabled:
        case kTestingModeUndefined:
        default:
        	callStaticVoidMethod("setTestingMode","(I)V",0);
        	break;
    }
#endif
}

TestingMode RevMobAds_Android::GetTestingMode()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	int mode = callStaticIntMethod("getTestingMode","()I");
    switch (mode) {
        case 0:
            return kTestingModeDisabled;
            break;
        case 1:
            return kTestingModeWithAds;
            break;
        case 2:
            return kTestingModeWithoutAds;
            break;
        default:
        	return kTestingModeUndefined;
    }
#else
    return kTestingModeUndefined;
#endif
}

void RevMobAds_Android::ShowFullscreen()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callStaticVoidMethod("showFullscreen","()V");
#endif
}

void RevMobAds_Android::ShowPopup()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callStaticVoidMethod("showPopup","()V");
#endif
}

void RevMobAds_Android::ShowBanner()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callStaticVoidMethod("showBanner","()V");
#endif
}

void RevMobAds_Android::HideBanner()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callStaticVoidMethod("hideBanner","()V");
#endif
}

void RevMobAds_Android::OpenLink()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callStaticVoidMethod("openLink","()V");
#endif
}


} /* namespace revmob */
