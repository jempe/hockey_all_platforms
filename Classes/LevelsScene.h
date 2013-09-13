#ifndef HOCKEY_PAUSED
#define HOCKEY_PAUSED "hockey_paused"
#endif

#ifndef LEVELSSCENE_H
#define LEVELSSCENE_H

#include "cocos2d.h"
#include "MenuSprite.h"
#include "CCLocalizedString.h"

class LevelsScene : public cocos2d::CCLayerGradient
{
    CCMenu * _levelsMenu;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(LevelsScene);

    ~LevelsScene(void);

    void StartEasy();
    void StartMedium();
    void StartHard();
    void StartGame();
    void GoBack();

    virtual void keyBackClicked();
};

#endif // LEVELSSCENE_H
