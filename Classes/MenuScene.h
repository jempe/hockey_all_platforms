#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "cocos2d.h"
#include "MenuSprite.h"
#include "CCLocalizedString.h"

class MenuScene : public cocos2d::CCLayerGradient
{
    CCMenu * _playMenu;
    CCSprite * _one_player;
    CCSprite * _two_players;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(MenuScene);

    ~MenuScene(void);

    void StartOnePlayer();

    void StartTwoPlayers();

    void StartGame(short int players);
};

#endif // MENUSCENE_H
