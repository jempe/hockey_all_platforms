#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

#include <vector>
#include <string>

#include "MenuScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	pDirector->setOpenGLView(pEGLView);

    CCSize screensize = pEGLView->getFrameSize();
    
    CCLog("screen width: %f", screensize.width);

	vector<string> searchPath;

    if(screensize.width < 321) // iphone 480x320
    {
        CCLog("Iphone resources");
        searchPath.push_back("iphone");
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    else if(screensize.width < 481) // nexus 800 x 480
    {
        CCLog("nexus s resources");
		searchPath.push_back("nexus");
    }
#endif
    else if(screensize.width < 769)  // iphone 5 1136x640
    {
        CCLog("iphone retina resources");
        searchPath.push_back("iphone5");
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    else if(screensize.width < 1001) // nexus 7 1280x800
    {
        CCLog("nexus 7 resources");
        searchPath.push_back("nexus7");
    }
    else if(screensize.width < 1201) // nexus 7 II 1920x1200
    {
        CCLog("nexus 7 II resources");
        searchPath.push_back("nexus7II");
    }
#endif
    else // nexus 10 2560x1600
    {
        CCLog("nexus 10 resources");
        searchPath.push_back("nexus10");
    }

    // set searching path
	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);

    //pDirector->setContentScaleFactor(4);

    std::string puck_effect = "puck.wav";
    char const * puck_effect_file = puck_effect.c_str();

    std::string puck_effect_location = CCFileUtils::sharedFileUtils()->fullPathForFilename(puck_effect_file);
    char const * puck_effect_location_file = puck_effect_location.c_str();

    SimpleAudioEngine::sharedEngine()->preloadEffect(puck_effect_location_file);

    std::string puck_effect_wall = "puck_wall.wav";
    char const * puck_effect_wall_file = puck_effect_wall.c_str();

    std::string puck_effect_wall_location = CCFileUtils::sharedFileUtils()->fullPathForFilename(puck_effect_wall_file);
    char const * puck_effect_wall_location_file = puck_effect_location.c_str();

    SimpleAudioEngine::sharedEngine()->preloadEffect(puck_effect_wall_location_file);

    std::string goal_effect = "goal.mp3";
    char const * goal_effect_file = goal_effect.c_str();

    std::string goal_effect_location = CCFileUtils::sharedFileUtils()->fullPathForFilename(goal_effect_file);
    char const * goal_effect_location_file = goal_effect_location.c_str();

    SimpleAudioEngine::sharedEngine()->preloadEffect(goal_effect_location_file);

    std::string button_effect = "button.wav";
    char const * button_effect_file = button_effect.c_str();

    std::string button_effect_location = CCFileUtils::sharedFileUtils()->fullPathForFilename(button_effect_file);
    char const * button_effect_location_file = button_effect_location.c_str();

    SimpleAudioEngine::sharedEngine()->preloadEffect(button_effect_location_file);

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MenuScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    CCNotificationCenter::sharedNotificationCenter()->postNotification(HOCKEY_PAUSED, NULL);
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
