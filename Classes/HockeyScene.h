#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <vector>
#include "cocos2d.h"
#include "VectorSprite.h"
#include "MenuSprite.h"
#include "CCLocalizedString.h"

class HockeyScene : public cocos2d::CCLayerColor
{
    const char * _font_file;

    CCSpriteBatchNode * _gameBatchNode;

	VectorSprite * _topPlayer;
	VectorSprite * _bottomPlayer;

	VectorSprite * _puck;

	short unsigned int _playersNumber;

    unsigned int _seconds_played;

	unsigned int _puck_ticks;
	unsigned int _puck_wall_ticks;

    std::vector<unsigned int> _high_scores;

	CCArray * _players;

	CCSize _screenSize;

	CCSprite * _table_bottom_right;
	CCSprite * _table_left;
	CCSprite * _center_circle;

    CCLabelTTF * _top_player_score;
    CCLabelTTF * _bottom_player_score;

	CCPoint _computer_mallet_rest;

	bool _gamePaused;
	bool _userMustResume;
	bool _goToPuck;

	bool _showingMenu;

	float _friction;

	short unsigned int _topPlayerScore;
	short unsigned int _bottomPlayerScore;

    unsigned int _playerScore;
    short unsigned int _current_score_digit;
    short unsigned int _score_digits;
    short unsigned int _current_digit_value;

	short unsigned int _computer_player_level;
	float _computer_mallet_speed;

    // elements of goal message
    CCLayer * _goal_message;
    CCSprite * _goal_message_background;
    CCArray * _goal_message_letters;

    CCLabelTTF * _final_score;

    CCLayerColor * _overlay;

    MenuSprite * _resume_button;
    MenuSprite * _resume_button_active;
    MenuSprite * _go_to_menu_button;
    MenuSprite * _go_to_menu_button_active;
    CCMenu * _pause_menu;
    CCMenuItemSprite * _resume_menu_item;
    CCMenuItemSprite * _go_to_menu_item;

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

    void timer();

    void resumeAfterGoal();

    void playAgain();

    void goBack();

    void getHighScores(const char *leaderboard);

    const char * getLeaderBoardName();

    unsigned int getScoreRanking(unsigned int score);

    void showLeaderboard();

    void showPauseMenu();

    void resumeGame();

    void generate_score();

    void showScoreCongrats();

    void showWinnerMenu();

    void playWallEffect();

    void showGoalLabel(short int player);

    void showWinnerLabel(short int player);

    void playerScore(short int player);

    void puckCollisionVector(CCPoint objectCenter, float objectRadius, CCPoint objectVector);

    CCPoint computerMalletPosition();

    CCPoint keepMalletInsideCourt(int player_id, CCPoint malletPosition);

    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);

    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);

    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);

    virtual void keyBackClicked();

    void flurry_event(std::string event_n);

    void save_moment(std::string moment_n);

    float log_2(float n);
};

#endif // __HELLOWORLD_SCENE_H__
