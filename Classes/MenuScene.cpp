#include "MenuScene.h"
#include "HockeyScene.h"
#include "LevelsScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MenuScene *layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool MenuScene::init()
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

    CCSprite * game_title = CCSprite::createWithSpriteFrameName("game_title.png");
    game_title->setPosition(ccp(screenSize.width / 2, screenSize.height * 0.75));
    game_title->setOpacity(0);
    _gameBatchNode->addChild(game_title);

    game_title->runAction(CCFadeIn::create(2.0f));

    _one_player = MenuSprite::createWithText(CCLocalizedString("ONEPLAYER"), false);
    _one_player->setPositionX(screenSize.width * 0.5 + (_one_player->getContentSize().width / 2));

    MenuSprite * one_player_active = MenuSprite::createWithText(CCLocalizedString("ONEPLAYER"), true);

    _two_players = MenuSprite::createWithText(CCLocalizedString("TWOPLAYERS"), false);
    _two_players->setPositionX(screenSize.width * 0.5 + (_two_players->getContentSize().width / 2));

    MenuSprite * two_players_active = MenuSprite::createWithText(CCLocalizedString("TWOPLAYERS"), true);

    CCMenuItemSprite * menu_one_player = CCMenuItemSprite::create(
                _one_player,
                one_player_active,
                this,
                menu_selector(MenuScene::StartOnePlayer)
                );

    CCMenuItemSprite * menu_two_players = CCMenuItemSprite::create(
                _two_players,
                two_players_active,
                this,
                menu_selector(MenuScene::StartTwoPlayers)
                );

    _playMenu = CCMenu::create(menu_one_player, menu_two_players, NULL);
    _playMenu->alignItemsVerticallyWithPadding(_two_players->getContentSize().height * 0.35);
    _playMenu->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.35));

    this->addChild(_playMenu);

    float move_button_time = 0.2;

    _one_player->runAction(CCMoveTo::create(move_button_time, ccp(0, _one_player->getPositionY())));
    _two_players->runAction(
                CCSequence::create(
                    CCScaleTo::create(move_button_time, 1.0f),
                    CCMoveTo::create(move_button_time, ccp(0, _two_players->getPositionY())),
                    NULL
                    )
                );

    return true;
}

void MenuScene::StartOnePlayer()
{
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    CCDirector::sharedDirector()->replaceScene(LevelsScene::scene());
}

void MenuScene::StartTwoPlayers()
{
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    StartGame(2);
}

void MenuScene::StartGame(short int players)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("number_of_players", players);
    CCDirector::sharedDirector()->replaceScene(HockeyScene::scene());
}

void MenuScene::keyBackClicked()
{
	CCLog("back key");
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

MenuScene::~MenuScene()
{
    //CC_SAFE_RELEASE(_players);
}
