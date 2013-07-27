#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "VectorSprite.h"

class HockeyScene : public cocos2d::CCLayerColor
{
	VectorSprite * _topPlayer;
	VectorSprite * _bottomPlayer;

	VectorSprite * _puck;

	short int _playersNumber;

	CCArray * _players;

	CCSize _screenSize;

	CCSprite * _table_bottom_right;
	CCSprite * _table_left;
	CCSprite * _center_circle;

	CCLabelTTF * _top_player_score;
	CCLabelTTF * _bottom_player_score;

	bool _gamePaused;

	float _friction;

	short int _topPlayerScore;
	short int _bottomPlayerScore;

	short int _computer_player_level;
	float _computer_mallet_speed;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HockeyScene);

    ~HockeyScene(void);

    void update(float dt);

    void playerScore(short int player);

    void puckCollisionVector(CCPoint objectCenter, float objectRadius, CCPoint objectVector);

    CCPoint computerMalletPosition();

    CCPoint keepMalletInsideCourt(int player_id, CCPoint malletPosition);

    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);

    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);

    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
};

#endif // __HELLOWORLD_SCENE_H__
