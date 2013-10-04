#include "LevelsScene.h"
#include "MenuScene.h"
#include "HockeyScene.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* LevelsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelsScene *layer = LevelsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool LevelsScene::init()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	 this->setKeypadEnabled(true);
#endif
    //////////////////////////////
    // 1. super init first
    if ( ! CCLayerGradient::initWithColor(ccc4(113, 152, 210, 255), ccc4(222, 231, 244, 255), ccp(0, -1)))
    {
        return false;
    }

    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 1);
    this->addChild(_gameBatchNode);

    MenuSprite * easy_button = MenuSprite::createWithText(CCLocalizedString("EASY"), false);
    easy_button->setPositionX(screenSize.width * 0.5 + (easy_button->getContentSize().width / 2));

    MenuSprite * easy_button_active = MenuSprite::createWithText(CCLocalizedString("EASY"), true);

    MenuSprite * medium_button = MenuSprite::createWithText(CCLocalizedString("MEDIUM"), false);
    medium_button->setPositionX(screenSize.width * 0.5 + (medium_button->getContentSize().width / 2));

    MenuSprite * medium_button_active = MenuSprite::createWithText(CCLocalizedString("MEDIUM"), true);

    MenuSprite * hard_button = MenuSprite::createWithText(CCLocalizedString("HARD"), false);
    hard_button->setPositionX(screenSize.width * 0.5 + (hard_button->getContentSize().width / 2));

    MenuSprite * hard_button_active = MenuSprite::createWithText(CCLocalizedString("HARD"), true);

    CCMenuItemSprite * easy_button_item = CCMenuItemSprite::create(
                easy_button,
                easy_button_active,
                this,
                menu_selector(LevelsScene::StartEasy)
                );

    CCMenuItemSprite * medium_button_item = CCMenuItemSprite::create(
                medium_button,
                medium_button_active,
                this,
                menu_selector(LevelsScene::StartMedium)
                );

    CCMenuItemSprite * hard_button_item = CCMenuItemSprite::create(
                hard_button,
                hard_button_active,
                this,
                menu_selector(LevelsScene::StartHard)
                );

    CCSprite * back_button = CCSprite::createWithSpriteFrameName("back_button.png");

    CCSprite * back_button_active = CCSprite::createWithSpriteFrameName("back_button_active.png");

    CCMenuItemSprite * back_button_item = CCMenuItemSprite::create(
                back_button,
                back_button_active,
                this,
                menu_selector(LevelsScene::GoBack)
                );

    CCMenu * back_menu = CCMenu::create(back_button_item, NULL);
    back_menu->setPosition(ccp(back_button->getContentSize().width * 0.7, screenSize.height - (back_button->getContentSize().height * 0.6)));

    this->addChild(back_menu);

    _levelsMenu = CCMenu::create(easy_button_item, medium_button_item, hard_button_item, NULL);
    _levelsMenu->alignItemsVerticallyWithPadding(easy_button->getContentSize().height * 0.2);
    _levelsMenu->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));

    this->addChild(_levelsMenu);

    float move_button_time = 0.2;

    easy_button->runAction(CCMoveTo::create(move_button_time, ccp(0, easy_button->getPositionY())));
    medium_button->runAction(
                CCSequence::create(
                    CCScaleTo::create(move_button_time, 1.0f),
                    CCMoveTo::create(move_button_time, ccp(0, medium_button->getPositionY())),
                    NULL
                    )
                );
    hard_button->runAction(
                CCSequence::create(
                    CCScaleTo::create(move_button_time * 2, 1.0f),
                    CCMoveTo::create(move_button_time, ccp(0, hard_button->getPositionY())),
                    NULL
                    )
                );

    return true;

}

void LevelsScene::GoBack()
{
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    CCDirector::sharedDirector()->replaceScene(MenuScene::scene());
}

void LevelsScene::StartEasy()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("computer_level", 1);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("computer_speed", 8);
    StartGame();
}

void LevelsScene::StartMedium()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("computer_level", 2);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("computer_speed", 5);
    StartGame();
}

void LevelsScene::StartHard()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("computer_level", 3);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("computer_speed", 2);
    StartGame();
}

void LevelsScene::StartGame()
{
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("number_of_players", 1);
    CCDirector::sharedDirector()->replaceScene(HockeyScene::scene());
}

void LevelsScene::keyBackClicked()
{
	GoBack();
}

LevelsScene::~LevelsScene()
{
    //CC_SAFE_RELEASE(_players);
}
