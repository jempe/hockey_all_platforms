#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

#include "cocos2d.h"
#include "MenuScene.h"
#include "HockeyScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCSize screensize = CCEGLView::sharedOpenGLView()->getFrameSize();

    if(screensize.width < 321) // iphone 480x320
    {
    	CCFileUtils::sharedFileUtils()->addSearchPath("iphone");
    }
    else if(screensize.width < 481) // nexus 800 x 480
    {
    	CCFileUtils::sharedFileUtils()->addSearchPath("nexus");
    }
    else if(screensize.width < 769)  // iphone 5 1136x640
    {
    	CCFileUtils::sharedFileUtils()->addSearchPath("iphone5");
    }
    else if(screensize.width < 1001) // nexus 7 1280x800
    {
        CCFileUtils::sharedFileUtils()->addSearchPath("nexus7");
    }
    else if(screensize.width < 1201) // nexus 7 II 1920x1200
    {
        CCFileUtils::sharedFileUtils()->addSearchPath("nexus7II");
    }
    else // nexus 10 2560x1600
    {
        CCFileUtils::sharedFileUtils()->addSearchPath("nexus10");
    }


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
    //CCScene *pScene = HockeyScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HOCKEY_PAUSED, NULL);
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
