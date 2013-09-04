#include "MenuScene.h"
#include "HockeyScene.h"
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
    //////////////////////////////
    // 1. super init first
    if ( ! CCLayerGradient::initWithColor(ccc4(113, 152, 210, 255), ccc4(222, 231, 244, 255), ccp(0, -1)))
    {
        return false;
    }

    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

    CCSprite * game_title = CCSprite::create("game_title.png");
    game_title->setPosition(ccp(screenSize.width / 2, screenSize.height * 0.75));
    game_title->setOpacity(0);
    this->addChild(game_title);

    game_title->runAction(CCFadeIn::create(2.0f));

    _one_player = MenuSprite::createWithText(CCLocalizedString("ONEPLAYER"));
    _one_player->setPositionX(screenSize.width * 0.5 + (_one_player->getContentSize().width / 2));

    _two_players = MenuSprite::createWithText(CCLocalizedString("TWOPLAYERS"));
    _two_players->setPositionX(screenSize.width * 0.5 + (_two_players->getContentSize().width / 2));

    CCMenuItemSprite * menu_one_player = CCMenuItemSprite::create(
                _one_player,
                _one_player,
                this,
                menu_selector(MenuScene::StartOnePlayer)
                );

    CCMenuItemSprite * menu_two_players = CCMenuItemSprite::create(
                _two_players,
                _two_players,
                this,
                menu_selector(MenuScene::StartTwoPlayers)
                );

    _playMenu = CCMenu::create(menu_one_player, menu_two_players, NULL);
    _playMenu->alignItemsVerticallyWithPadding(_two_players->getContentSize().height * 0.2);
    _playMenu->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));

    this->addChild(_playMenu);

    _one_player->runAction(CCMoveTo::create(0.1, ccp(0, _one_player->getPositionY())));
    _two_players->runAction(
                CCSequence::create(
                    CCMoveTo::create(0.1, ccp(_two_players->getPositionX(), _two_players->getPositionY())),
                    CCMoveTo::create(0.1, ccp(0, _two_players->getPositionY())),
                    NULL
                    )
                );

    return true;
}

void MenuScene::StartOnePlayer()
{
    StartGame(1);
}

void MenuScene::StartTwoPlayers()
{
    StartGame(2);
}

void MenuScene::StartGame(short int players)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("number_of_players", players);
    CCDirector::sharedDirector()->replaceScene(HockeyScene::scene());
}

MenuScene::~MenuScene()
{
    //CC_SAFE_RELEASE(_players);
}
