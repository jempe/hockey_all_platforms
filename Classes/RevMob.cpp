#include "RevMob.h"
#include "platform/CCPlatformMacros.h"

#ifndef CC_TARGET_PLATFORM
#error "CC_TARGET_PLATFORM should be defined!!!"
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include "RevMobAds_iOS.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include "RevMobAds_Android.h"
#endif

namespace revmob{

    RevMob* RevMob::sSharedInstance = 0;

    RevMob::RevMob(){};

    RevMob::~RevMob(){};

    RevMob *RevMob::SharedInstance(){

        if (RevMob::sSharedInstance == 0) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            RevMob::sSharedInstance = new RevMobAds_Android();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            RevMob::sSharedInstance = new RevMobAds_iOS();
#else
            RevMob::sSharedInstance = 0;
#endif

        }

        
        return RevMob::sSharedInstance;
    };

}





