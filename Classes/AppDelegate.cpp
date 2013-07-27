#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

#include "cocos2d.h"
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
    else if(screensize.width < 641)  // iphone 5 1136x640
    {
    	CCFileUtils::sharedFileUtils()->addSearchPath("iphone5");
    }
    else if(screensize.width < 801) // nexus 7 1280x800
    {
    	CCFileUtils::sharedFileUtils()->addSearchPath("nexus7");
    }
    else // nexus 10 2560x1600
    {
    	CCFileUtils::sharedFileUtils()->addSearchPath("nexus10");
    }

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HockeyScene::scene();

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
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
