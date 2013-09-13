#include "HockeyScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

static const float PI = 3.1416;

CCScene* HockeyScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HockeyScene *layer = HockeyScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HockeyScene::init()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	 this->setKeypadEnabled(true);
#endif

	 _puck_ticks = 0;
	 _puck_wall_ticks = 0;
	_showingMenu = false;
	_userMustResume = false;
	_gamePaused = false;
	_goToPuck = true;
    _playersNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("number_of_players");
    _friction = 0.987;
	_bottomPlayerScore = 0;
	_topPlayerScore = 0;
    _computer_player_level = CCUserDefault::sharedUserDefault()->getIntegerForKey("computer_level");

    //////////////////////////////
    // 1. super init first
    if ( ! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }

    _screenSize = CCDirector::sharedDirector()->getWinSize();

    _computer_mallet_rest = ccp(_screenSize.width / 2, _screenSize.height * 0.75);

    // create court items
    _center_circle = CCSprite::create("circle.png");
    _center_circle->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(_center_circle);
    
    CCSprite * center_line = CCSprite::create("line.png");
    center_line->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(center_line);

    CCSprite * bottom_circle = CCSprite::create("circle.png");
    bottom_circle->setPosition(ccp(_screenSize.width * 0.5, 0));
    this->addChild(bottom_circle);

    CCSprite * top_circle = CCSprite::create("circle.png");
    top_circle->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height));
    this->addChild(top_circle);

    CCSprite * logo = CCSprite::create("jempe_logo.png");
    logo->setPosition(ccp(_screenSize.width / 2, (_screenSize.height / 2) + (logo->getContentSize().height / 8)));
    this->addChild(logo);

    //create table borders
    _table_left = CCSprite::create("table_side.png");
    _table_left->setPosition(ccp(_table_left->getContentSize().width * 0.5, _screenSize.height * 0.5));
    this->addChild(_table_left);

    CCSprite * table_right = CCSprite::create("table_side.png");
    table_right->setPosition(ccp(_screenSize.width - table_right->getContentSize().width * 0.5, _screenSize.height * 0.5));
    table_right->setRotation(180);
    this->addChild(table_right);

    _table_bottom_right = CCSprite::create("table_bottom.png");
    _table_bottom_right->setPosition(ccp(_screenSize.width + ((_table_bottom_right->getContentSize().width - _screenSize.width) / 2) + (_center_circle->getContentSize().width * 0.5), _table_bottom_right->getContentSize().height * 0.5));
    this->addChild(_table_bottom_right);

    CCSprite * table_bottom_left = CCSprite::create("table_bottom.png");
    table_bottom_left->setPosition(ccp(- ((_table_bottom_right->getContentSize().width - _screenSize.width) / 2) - (_center_circle->getContentSize().width * 0.5), table_bottom_left->getContentSize().height * 0.5));
    table_bottom_left->setScaleX(-1);
    this->addChild(table_bottom_left);

    CCSprite * table_top_right = CCSprite::create("table_bottom.png");
    table_top_right->setPosition(ccp(_screenSize.width + ((_table_bottom_right->getContentSize().width - _screenSize.width) / 2) + (_center_circle->getContentSize().width * 0.5), _screenSize.height - (table_top_right->getContentSize().height * 0.5)));
    table_top_right->setScaleY(-1);
    this->addChild(table_top_right);

    CCSprite * table_top_left = CCSprite::create("table_bottom.png");
    table_top_left->setPosition(ccp(- ((_table_bottom_right->getContentSize().width - _screenSize.width) / 2) - (_center_circle->getContentSize().width * 0.5), _screenSize.height - (table_top_left->getContentSize().height * 0.5)));
    table_top_left->setScaleY(-1);
    table_top_left->setScaleX(-1);
    this->addChild(table_top_left);


    // add corners
    CCSprite * corner_top_left = CCSprite::create("corner.png");
    corner_top_left->setPosition(ccp((corner_top_left->getContentSize().width / 2), _screenSize.height - (corner_top_left->getContentSize().height / 2)));
    this->addChild(corner_top_left);

    CCSprite * corner_top_right = CCSprite::create("corner.png");
    corner_top_right->setPosition(ccp(_screenSize.width - (corner_top_right->getContentSize().width / 2), _screenSize.height - (corner_top_right->getContentSize().height / 2)));
    corner_top_right->setRotation(90);
    this->addChild(corner_top_right);

    CCSprite * corner_bottom_right = CCSprite::create("corner.png");
    corner_bottom_right->setPosition(ccp(_screenSize.width - (corner_bottom_right->getContentSize().width / 2), (corner_bottom_right->getContentSize().height / 2)));
    corner_bottom_right->setRotation(180);
    this->addChild(corner_bottom_right);

    CCSprite * corner_bottom_left = CCSprite::create("corner.png");
    corner_bottom_left->setPosition(ccp((corner_bottom_left->getContentSize().width / 2), (corner_bottom_left->getContentSize().height / 2)));
    corner_bottom_left->setRotation(270);
    this->addChild(corner_bottom_left);

    // 30 for small screens, 60 for medium screens

    // create players mallets
    _topPlayer = VectorSprite::vectorSpriteWithFile("mallet.png");
    _topPlayer->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.75));
    if(_playersNumber > 1)
    {
        _topPlayer->setOpacity(128);
    }
        this->addChild(_topPlayer);

    _bottomPlayer = VectorSprite::vectorSpriteWithFile("mallet.png");
    _bottomPlayer->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.25));
    _bottomPlayer->setOpacity(128);
    this->addChild(_bottomPlayer);

    _computer_mallet_speed = _topPlayer->get_radius() / CCUserDefault::sharedUserDefault()->getIntegerForKey("computer_speed");

    _players = CCArray::create(_bottomPlayer, _topPlayer, NULL);
    _players->retain();

    // create score labels
    _top_player_score = CCLabelBMFont::create("0", "londrina_solid.fnt", 10);
    _top_player_score->setPosition(ccp(_screenSize.width - _bottomPlayer->get_radius(), (_screenSize.height / 2) + _bottomPlayer->get_radius()));
    _top_player_score->setColor(ccc3(255, 0, 0));
    this->addChild(_top_player_score);

    _bottom_player_score = CCLabelBMFont::create("0", "londrina_solid.fnt", 10);
    _bottom_player_score->setPosition(ccp(_screenSize.width - _bottomPlayer->get_radius(), (_screenSize.height / 2) - (_bottomPlayer->get_radius() * 0.7)));
    _bottom_player_score->setColor(ccc3(255, 0, 0));
    this->addChild(_bottom_player_score);

    // create puck
    _puck = VectorSprite::vectorSpriteWithFile("puck.png");
    _puck->setPosition(ccp(_screenSize.width / 2, _screenSize.height / 2));
    //_puck->setPosition(ccp(_screenSize.width / 2, _screenSize.height + (_puck->get_radius() * 0.75)));
    //_puck->setPosition(ccp(_table_left->getContentSize().width + _puck->get_radius(), _screenSize.height - _table_bottom_right->getContentSize().height - _topPlayer->get_radius()));
    this->addChild(_puck);

    // create the goal message layer
    _goal_message = new CCLayer();
    _goal_message->setPosition(ccp(_screenSize.width / 2, _screenSize.height / 2));

    // select the background according to the system language
    _goal_message_background = CCSprite::create(CCLocalizedString("GOALBACKGROUND"));
    _goal_message->addChild(_goal_message_background);

    CCSprite * goal_space = CCSprite::create("goal_border_letters_separation.png");
    _goal_message->addChild(goal_space);

    float letter_position_x = -(_goal_message_background->getContentSize().width / 2) + goal_space->getContentSize().width;
    float letter_position_y = (_goal_message_background->getContentSize().height / 2) - goal_space->getContentSize().height;

    const char * goalName = CCLocalizedString("GOALNAME");

    _goal_message_letters = CCArray::create();

    for(long unsigned int i = 0; i < strlen(goalName); i++)
    {
        char letter_file_name[] = {'g', 'o', 'a', 'l', '_', goalName[i], '.', 'p', 'n', 'g', '\0'};

        //CCLog("%s", letter_file_name);

        CCSprite * letter = CCSprite::create(letter_file_name);

        letter_position_x += letter->getContentSize().width / 2;

        letter->setPosition(ccp(letter_position_x, letter_position_y - (letter->getContentSize().height / 2)));

        letter_position_x += letter->getContentSize().width / 2;

        _goal_message->addChild(letter);
        _goal_message_letters->addObject(letter);
    }

    _goal_message_letters->retain();

    this->addChild(_goal_message);

    _goal_message->setVisible(false);

    //overlay
    _overlay = new CCLayerColor();
    _overlay->initWithColor(ccc4(0, 0, 0, 128));
    _overlay->setContentSize(CCSizeMake(_screenSize.width, _screenSize.height));
    this->addChild(_overlay);
    _overlay->setVisible(false);

    // create pause menu and hide it

	_resume_button = MenuSprite::createWithText(CCLocalizedString("RESUME"), false);
	_resume_button_active = MenuSprite::createWithText(CCLocalizedString("RESUME"), true);
	_go_to_menu_button = MenuSprite::createWithText(CCLocalizedString("GOBACK"), false);
	_go_to_menu_button_active = MenuSprite::createWithText(CCLocalizedString("GOBACK"), true);

	_resume_menu_item = CCMenuItemSprite::create(
			_resume_button,
			_resume_button_active,
			this,
			menu_selector(HockeyScene::resumeGame)
	);

	_go_to_menu_item = CCMenuItemSprite::create(
			_go_to_menu_button,
			_go_to_menu_button_active,
			this,
			menu_selector(HockeyScene::goBack)
	);

	_pause_menu = CCMenu::create(_resume_menu_item, _go_to_menu_item, NULL);
	_pause_menu->alignItemsVerticallyWithPadding(_resume_button->getContentSize().height);
	_pause_menu->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));

	this->addChild(_pause_menu);
	_pause_menu->setVisible(false);


    // listen for touches
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(HockeyScene::update));

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HockeyScene::showPauseMenu), HOCKEY_PAUSED, NULL);

    return true;
}

void HockeyScene::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;
	VectorSprite * player;

	if( ! _gamePaused)
	{
		for(i = touches->begin(); i != touches->end(); i++)
		{
			touch = (CCTouch*) (*i);

			if(touch)
			{
				tap = touch->getLocation();

				for(short int j = 0; j < _playersNumber; j++)
				{
					player = (VectorSprite *) _players->objectAtIndex(j);

					if(player->boundingBox().containsPoint(tap))
					{
						player->setTouch(touch);
					}
                    else if(_bottomPlayer->getTouch() == NULL && tap.y < _screenSize.height / 2)
                    {
                        tap = keepMalletInsideCourt(0, tap);
                        _bottomPlayer->setPosition(tap);
                        _bottomPlayer->setNextPos(tap);

                        _bottomPlayer->setTouch(touch);
                    }
                    else if(_playersNumber > 1 && _topPlayer->getTouch() == NULL && tap.y > _screenSize.height / 2)
                    {
                        tap = keepMalletInsideCourt(1, tap);
                        _topPlayer->setPosition(tap);
                        _topPlayer->setNextPos(tap);

                        _topPlayer->setTouch(touch);
                    }
				}
			}
		}
	}
}

void HockeyScene::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;
	VectorSprite * player;

	if( ! _gamePaused)
	{
		for(i = touches->begin(); i != touches->end(); i++)
		{
			touch = (CCTouch*) (*i);

			if(touch)
			{
				tap = touch->getLocation();

				for(short int j = 0; j < _playersNumber; j++)
				{
					player = (VectorSprite *) _players->objectAtIndex(j);

					if(player->getTouch() != NULL && player->getTouch() == touch)
					{
						CCPoint nextPos = tap;

						nextPos = keepMalletInsideCourt(j, nextPos);

						player->setNextPos(nextPos);
						player->setVector(ccp(nextPos.x - player->getPositionX(), nextPos.y - player->getPositionY()));
					}
				}
			}
		}
	}
}

void HockeyScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;
	VectorSprite * player;

	if( ! _gamePaused)
	{
		for(i = touches->begin(); i != touches->end(); i++)
		{
			touch = (CCTouch*) (*i);

			if(touch)
			{
				tap = touch->getLocation();

				for(short int j = 0; j < _playersNumber; j++)
				{
					player = (VectorSprite *) _players->objectAtIndex(j);

					if(player->getTouch() != NULL && player->getTouch() == touch)
					{
						player->setTouch(NULL);
						player->setVector(ccp(0, 0));
					}
				}
			}
		}
	}
}

/********************************************//**
 *  Method that checks the position of mallets and puck and update the position and vectors
 ***********************************************/

void HockeyScene::update(float dt)
{
	_puck_ticks++;
	_puck_wall_ticks++;
	VectorSprite * player;

	if( ! _gamePaused)
	{
		/**
		 * detects collisions with mallets
		 */
		for(short unsigned int j = 0; j < _players->count(); j++)
		{
			player = (VectorSprite *) _players->objectAtIndex(j);

			if(j < _playersNumber)
			{
				if(player->getTouch() == NULL)
				{
					player->setOpacity(128);
				}
				else
				{
					player->setOpacity(255);
				}
			}

			CCPoint player_position = player->getNextPos();

			puckCollisionVector(player_position, player->get_radius(), player->getVector());
		}

		_puck->setVector(ccpMult(_puck->getVector(), _friction));

		/**
		 * detect and handle collisions with the sides of the table
		 */

		CCPoint puck_next_position = ccpAdd(_puck->getPosition(), _puck->getVector());
		CCPoint current_puck_vector = _puck->getVector();

		if(puck_next_position.x < _table_left->getContentSize().width + _puck->get_radius())
		{
			playWallEffect();

			puck_next_position.x = _table_left->getContentSize().width + _puck->get_radius();

			if(current_puck_vector.x < 0)
			{
				_puck->setVector(ccp(- current_puck_vector.x, current_puck_vector.y));
			}
		}
		else if(puck_next_position.x > _screenSize.width - _table_left->getContentSize().width - _puck->get_radius())
		{
			playWallEffect();
			puck_next_position.x = _screenSize.width - _table_left->getContentSize().width - _puck->get_radius();

			if(current_puck_vector.x > 0)
			{
				_puck->setVector(ccp(- current_puck_vector.x, current_puck_vector.y));
			}
		}

		/**
		 * detect and handle collisions with the sides of the goal
		 */

		if((puck_next_position.x < (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() || puck_next_position.x > (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius()) && (puck_next_position.y < 0 || puck_next_position.y > _screenSize.height))
		{
			playWallEffect();
			_puck->setVector(ccp(-current_puck_vector.x, current_puck_vector.y));
		}

		/**
		 * detect and handle collisions with bottom and top of the table
		 */

		if(puck_next_position.x < (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() - _table_bottom_right->getContentSize().height || puck_next_position.x > (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius() + _table_bottom_right->getContentSize().height)
		{
			if(puck_next_position.y < _table_bottom_right->getContentSize().height + _puck->get_radius())
			{
				playWallEffect();
				puck_next_position.y = _table_bottom_right->getContentSize().height + _puck->get_radius();

				if(current_puck_vector.y < 0)
				{
					_puck->setVector(ccp(current_puck_vector.x, -current_puck_vector.y));
				}
			}
			else if(puck_next_position.y > _screenSize.height -_table_bottom_right->getContentSize().height - _puck->get_radius())
			{
				playWallEffect();
				puck_next_position.y = _screenSize.height -_table_bottom_right->getContentSize().height - _puck->get_radius();

				if(current_puck_vector.y > 0)
				{
					_puck->setVector(ccp(current_puck_vector.x, -current_puck_vector.y));
				}
			}
		}

		/**
		 * detect and handle collisions with goal corners
		 */

		// center points of the goal corners
		CCPoint goal_bottom_left = ccp((_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) - _table_bottom_right->getContentSize().height, 0);
		CCPoint goal_bottom_right = ccp((_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) + _table_bottom_right->getContentSize().height, 0);
		CCPoint goal_top_left = ccp((_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) - _table_bottom_right->getContentSize().height, _screenSize.height);
		CCPoint goal_top_right = ccp((_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) + _table_bottom_right->getContentSize().height, _screenSize.height);

		if(puck_next_position.y < _table_bottom_right->getContentSize().height + _puck->get_radius())
		{
			if(puck_next_position.x < _screenSize.width / 2)
			{
				puckCollisionVector(goal_bottom_left, _table_bottom_right->getContentSize().height, CCPoint (0, 0));
			}
			else
			{
				puckCollisionVector(goal_bottom_right, _table_bottom_right->getContentSize().height, CCPoint (0, 0));
			}
		}
		else if(puck_next_position.y > _screenSize.height - _table_bottom_right->getContentSize().height - _puck->get_radius())
		{
			if(puck_next_position.x < _screenSize.width / 2)
			{
				puckCollisionVector(goal_top_left, _table_bottom_right->getContentSize().height, CCPoint (0, 0));
			}
			else
			{
				puckCollisionVector(goal_top_right, _table_bottom_right->getContentSize().height, CCPoint (0, 0));
			}
		}

		/**
		 * move puck to next position
		 */
		_puck->setPosition(puck_next_position);

		/**
		 * move mallets to next position
		 */

		_bottomPlayer->setPosition(_bottomPlayer->getNextPos());

		if(_playersNumber > 1)
		{
			_topPlayer->setPosition(_topPlayer->getNextPos());
		}
		else if( ! _gamePaused)
		{
			CCPoint next_computer_mallet_position = computerMalletPosition();

			_topPlayer->setVector(ccp(next_computer_mallet_position.x - _topPlayer->getPositionX(), next_computer_mallet_position.y - _topPlayer->getPositionY()));
			_topPlayer->setPosition(next_computer_mallet_position);
		}

		/**
		 * detect goals
		 */

		if(_puck->getPositionY() < -_puck->get_radius())
		{
			playerScore(2);
		}

		if(_puck->getPositionY() > _screenSize.height + _puck->get_radius())
		{
			playerScore(1);
		}
	}
}

void HockeyScene::playWallEffect()
{
	if(_puck_wall_ticks > 10) // avoid that effect is played more than once every collision
	{
			SimpleAudioEngine::sharedEngine()->playEffect("puck_wall.wav");
			_puck_wall_ticks = 0;
	}
}

/********************************************//**
 *  AI for computer mallet
 ***********************************************/
CCPoint HockeyScene::computerMalletPosition()
{
	CCPoint mallet_position = _topPlayer->getPosition();

	if(_puck->getPositionY() > _screenSize.height / 2)
	{
		if(_goToPuck)
        {
			if(abs(mallet_position.x - _puck->getPositionX()) > _computer_mallet_speed)
			{
				if(mallet_position.x > _puck->getPositionX())
				{
					mallet_position.x -= _computer_mallet_speed;
				}
				else
				{
					mallet_position.x += _computer_mallet_speed;
				}
			}

			if(abs(mallet_position.y - _puck->getPositionY()) > _computer_mallet_speed)
			{
				if(mallet_position.y > _puck->getPositionY())
				{
					mallet_position.y -= _computer_mallet_speed;
				}
				else
				{
					mallet_position.y += _computer_mallet_speed;
				}
			}
		}
		else
		{
			float return_speed = 15;

			mallet_position.x += (_computer_mallet_rest.x - mallet_position.x) / return_speed;
			mallet_position.y += (_computer_mallet_rest.y - mallet_position.y) / return_speed;
		}

		if(pow(mallet_position.x - _puck->getPositionX(),2) + pow(mallet_position.y - _puck->getPositionY(), 2) < pow((_topPlayer->get_radius() + _puck->get_radius() * 0.9), 2))
		{
			_goToPuck = false;

			if(_puck->getPositionX() > (_screenSize.width / 2))
			{
				_computer_mallet_rest.x = _puck->getPositionX() - (_topPlayer->get_radius() * 2);
			}
			else
			{
				_computer_mallet_rest.x = _puck->getPositionX() + (_topPlayer->get_radius() * 2);
			}

			_computer_mallet_rest.y = _topPlayer->getPositionY() + (_topPlayer->get_radius() * 2);

            /********************************************//**
             *  if the puck is in the corner random position will help to send the puck to the other side
             ***********************************************/

            if(_computer_mallet_rest.y > (_screenSize.height - _table_bottom_right->getContentSize().height - _topPlayer->get_radius()))
            {
                _computer_mallet_rest.y = (_screenSize.height - _table_bottom_right->getContentSize().height - _topPlayer->get_radius()) - (((double) rand() / (RAND_MAX)) * _topPlayer->get_radius() * 1.8);
            }

            //_computer_mallet_rest = keepMalletInsideCourt(1, _computer_mallet_rest);
		}

		if(pow(mallet_position.x - _computer_mallet_rest.x,2) + pow(mallet_position.y - _computer_mallet_rest.y, 2) < pow(_topPlayer->get_radius() / 4, 2))
		{
			_goToPuck = true;
		}
	}
	else
	{
		_goToPuck = true;
		float return_speed = 15;

		mallet_position.x += ((_screenSize.width / 2) - mallet_position.x) / return_speed;
		mallet_position.y += ((_screenSize.height * 0.75) - mallet_position.y) / return_speed;
	}

    CCLog("mallet position y: %f", mallet_position.y);

	mallet_position = keepMalletInsideCourt(1, mallet_position);

    CCLog("mallet position after keep inside court y: %f", mallet_position.y);

	return mallet_position;
}
void HockeyScene::resumeAfterGoal()
{
	if( ! _userMustResume)
	{
		_gamePaused = false;
	}
    _goal_message->setVisible(false);
}

void HockeyScene::showPauseMenu()
{
	if( ! _showingMenu)
	{
		_gamePaused = true;
		_userMustResume = true;
		_showingMenu = true;
		_overlay->setVisible(true);
		_pause_menu->setVisible(true);
	}
}

void HockeyScene::resumeGame()
{
	_overlay->setVisible(false);
	_userMustResume = false;
	_showingMenu = false;
	_gamePaused = false;
	_pause_menu->setVisible(false);
}

void HockeyScene::showWinnerMenu()
{
	_showingMenu = true;
	_overlay->setVisible(true);

	MenuSprite * again = MenuSprite::createWithText(CCLocalizedString("PLAYAGAIN"), false);
	again->setPositionX(_screenSize.width * 0.5 + (again->getContentSize().width / 2));

	MenuSprite * again_active = MenuSprite::createWithText(CCLocalizedString("PLAYAGAIN"), true);

	MenuSprite * go_back = MenuSprite::createWithText(CCLocalizedString("GOBACK"), false);
	go_back->setPositionX(_screenSize.width * 0.5 + (go_back->getContentSize().width / 2));

	MenuSprite * go_back_active = MenuSprite::createWithText(CCLocalizedString("GOBACK"), true);

	CCMenuItemSprite * menu_again = CCMenuItemSprite::create(
			again,
			again_active,
			this,
			menu_selector(HockeyScene::playAgain)
	);

	CCMenuItemSprite * menu_goback = CCMenuItemSprite::create(
			go_back,
			go_back_active,
			this,
			menu_selector(HockeyScene::goBack)
	);

	CCMenu * winMenu = CCMenu::create(menu_again, menu_goback, NULL);
	winMenu->alignItemsVerticallyWithPadding(again->getContentSize().height * 0.4);
	winMenu->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));

	this->addChild(winMenu);

	float move_button_time = 0.2;

	again->runAction(CCMoveTo::create(move_button_time, ccp(0, again->getPositionY())));
	go_back->runAction(
			CCSequence::create(
					CCScaleTo::create(move_button_time, 1.0),
					CCMoveTo::create(move_button_time, ccp(0, go_back->getPositionY())),
					NULL
			)
	);
}

void HockeyScene::showGoalLabel(short int player)
{
    _gamePaused = true;
    _goal_message->setVisible(true);

    _goal_message_background->setOpacity(0);

    if(player > 1)
    {
        _goal_message->setPositionY(_screenSize.height * 0.65);
        _goal_message->setRotation(180);
    }
    else
    {
        _goal_message->setPositionY(_screenSize.height * 0.35);
        _goal_message->setRotation(0);
    }

    CCFiniteTimeAction* goal_label_done = CCCallFuncN::create( this,
                                            callfuncN_selector(HockeyScene::resumeAfterGoal));

    float fade_time = 0.2f;
    float scale_time = 0.1f;
    float shake_time = 0.1f;
    short int shakes = 3;

    float letter_animation_time = (scale_time * 2) +  (_goal_message_letters->count() *(shakes * shake_time));

    CCSequence * goal_message_sequence =
            CCSequence::create(
                CCFadeIn::create(fade_time),
                CCScaleTo::create(letter_animation_time, 1.0f),
                CCFadeOut::create(fade_time),
                goal_label_done,
                NULL
                );

    CCSequence * letter_sequence[_goal_message_letters->count()];

    for(unsigned int l =  0; l < _goal_message_letters->count(); l++)
    {
        CCSprite * letter = (CCSprite *) _goal_message_letters->objectAtIndex(l);
        letter->setOpacity(0);

        letter_sequence[l] =
                CCSequence::create(
                    CCScaleTo::create(fade_time + (l * scale_time), 1.0f),
                    CCSpawn::create(
                        CCFadeIn::create(scale_time),
                        CCScaleTo::create(scale_time, 1.5f),
                        CCRotateTo::create(scale_time, 15),
                        NULL
                        ),
                    CCRotateTo::create(shake_time, -15),
                    CCSpawn::create(
                        CCScaleTo::create(scale_time, 1.0f),
                        CCRotateTo::create(scale_time, 0),
                        NULL
                        ),
                    CCScaleTo::create(letter_animation_time - (l * scale_time) - (scale_time * 2) - shake_time, 1.0f),
                    CCFadeOut::create(fade_time),
                    NULL
                    );

        letter->runAction(letter_sequence[l]);
    }

    _goal_message_background->runAction(goal_message_sequence);
}

void HockeyScene::showWinnerLabel(short int player)
{
	_showingMenu = true;

    // create the winner message layer
    CCLayer * winner_message = new CCLayer();
    winner_message->setPosition(ccp(_screenSize.width / 2, _screenSize.height / 2));

    // select the background according to the system language
    CCSprite * winner_message_background = CCSprite::create(CCLocalizedString("WINNERBACKGROUND"));
    winner_message->addChild(winner_message_background);

    CCSprite * winner_space = CCSprite::create("goal_border_letters_separation.png");
    winner_message->addChild(winner_space);

    float letter_position_x = -(winner_message_background->getContentSize().width / 2) + winner_space->getContentSize().width;
    float letter_position_y = (winner_message_background->getContentSize().height / 2) - winner_space->getContentSize().height;

    const char * winnerName = CCLocalizedString("WINNERNAME");

    CCArray * winner_message_letters = CCArray::create();

    for(long unsigned int i = 0; i < strlen(winnerName); i++)
    {
        char letter_file_name[] = {'g', 'o', 'a', 'l', '_', winnerName[i], '.', 'p', 'n', 'g', '\0'};

        CCLog("%s", letter_file_name);

        CCSprite * letter = CCSprite::create(letter_file_name);

        letter_position_x += letter->getContentSize().width / 2;

        letter->setPosition(ccp(letter_position_x, letter_position_y - (letter->getContentSize().height / 2)));

        letter_position_x += letter->getContentSize().width / 2;

        winner_message->addChild(letter);
        winner_message_letters->addObject(letter);
    }

    this->addChild(winner_message);

    _gamePaused = true;

    winner_message_background->setOpacity(0);

    if(player > 1)
    {
        winner_message->setPositionY(_screenSize.height * 0.65);
        winner_message->setRotation(180);
    }
    else
    {
        winner_message->setPositionY(_screenSize.height * 0.35);
        winner_message->setRotation(0);
    }

    CCFiniteTimeAction* winner_label_done = CCCallFuncN::create( this,
                                            callfuncN_selector(HockeyScene::showWinnerMenu));

    float fade_time = 0.2f;
    float scale_time = 0.1f;
    float shake_time = 0.1f;
    short int shakes = 3;

    float letter_animation_time = (scale_time * 2) +  (winner_message_letters->count() *(shakes * shake_time));

    CCSequence * winner_message_sequence =
            CCSequence::create(
                CCFadeIn::create(fade_time),
                CCScaleTo::create(letter_animation_time + fade_time, 1.0f),
                winner_label_done,
                NULL
                );

    CCSequence * letter_sequence[winner_message_letters->count()];

    for(unsigned int l =  0; l < winner_message_letters->count(); l++)
    {
        CCSprite * letter = (CCSprite *) winner_message_letters->objectAtIndex(l);
        letter->setOpacity(0);

        letter_sequence[l] =
                CCSequence::create(
                    CCScaleTo::create(fade_time + (l * scale_time), 1.0f),
                    CCSpawn::create(
                        CCFadeIn::create(scale_time),
                        CCScaleTo::create(scale_time, 1.5f),
                        CCRotateTo::create(scale_time, 15),
                        NULL
                        ),
                    CCRotateTo::create(shake_time, -15),
                    CCSpawn::create(
                        CCScaleTo::create(scale_time, 1.0f),
                        CCRotateTo::create(scale_time, 0),
                        NULL
                        ),
                    CCScaleTo::create(letter_animation_time - (l * scale_time) - (scale_time * 2) - shake_time + fade_time, 1.0f),
                    NULL
                    );

        letter->runAction(letter_sequence[l]);
    }

    winner_message_background->runAction(winner_message_sequence);
}

void HockeyScene::playAgain()
{
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    CCDirector::sharedDirector()->replaceScene(HockeyScene::scene());
}

void HockeyScene::goBack()
{
    SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
    CCDirector::sharedDirector()->replaceScene(MenuScene::scene());
}

/********************************************//**
 *  Update player score
 *  @param player int of the player
 *  	0 -> bottom player
 *  	1 -> top player
 ***********************************************/

void HockeyScene::playerScore(short int player)
{
	SimpleAudioEngine::sharedEngine()->playEffect("goal.mp3");

    _goToPuck = true;
	CCPoint center;

	if(player > 1)
	{
		_topPlayerScore++;
		center = ccp(_screenSize.width / 2, _screenSize.height / 2 - _puck->get_radius());
	}
	else
	{
		_bottomPlayerScore++;
		center = ccp(_screenSize.width / 2, _screenSize.height / 2 + _puck->get_radius());
	}



    if(_bottomPlayerScore == 7 || _topPlayerScore == 7)
    {
        showWinnerLabel(player);
    }
    else
    {
        showGoalLabel(player);
    }

	/**
	 *  display score
	 */

	char score[10];

	sprintf(score,"%d", _topPlayerScore);
	_top_player_score->setString(score);

	sprintf(score,"%d", _bottomPlayerScore);
	_bottom_player_score->setString(score);

    //CCLog("top: %d bottom: %d", _topPlayerScore, _bottomPlayerScore);

	_puck->setVector(ccp(0, 0));

	_puck->setNextPos(center);
	_puck->setPosition(center);

	CCPoint top_player_pos = ccp(_screenSize.width / 2, (_screenSize.height * 3) / 4);
	CCPoint bottom_player_pos = ccp(_screenSize.width / 2, _screenSize.height / 4);

	_topPlayer->setPosition(top_player_pos);
	_topPlayer->setNextPos(top_player_pos);
	_topPlayer->setTouch(NULL);

	_bottomPlayer->setPosition(bottom_player_pos);
	_bottomPlayer->setNextPos(bottom_player_pos);
	_bottomPlayer->setTouch(NULL);
}

/********************************************//**
 *  SCENE destructor
 ***********************************************/

HockeyScene::~HockeyScene()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, HOCKEY_PAUSED);
	CC_SAFE_RELEASE(_players);
	CC_SAFE_RELEASE(_goal_message_letters);
}

void HockeyScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

/********************************************//**
 *  keep the mallet inside the court
 *  @param player_id int of the player
 *  	0 -> bottom player
 *  	1 -> top player
 *  @param malletPosition coordinates of the requested mallet position
 ***********************************************/

CCPoint HockeyScene::keepMalletInsideCourt(int player_id, CCPoint malletPosition)
{
	if(malletPosition.x < _table_left->getContentSize().width + _topPlayer->get_radius())
	{
		malletPosition.x = _table_left->getContentSize().width + _topPlayer->get_radius();
	}

	if(malletPosition.x > _screenSize.width - _table_left->getContentSize().width - _topPlayer->get_radius())
	{
		malletPosition.x = _screenSize.width - _table_left->getContentSize().width - _topPlayer->get_radius();
	}

	if(player_id == 0)
	{
		if(malletPosition.y > (_screenSize.height / 2) - _topPlayer->get_radius())
		{
			malletPosition.y = (_screenSize.height / 2) - _topPlayer->get_radius();
		}

		if(malletPosition.y < (_topPlayer->get_radius() + _table_bottom_right->getContentSize().height))
		{
            if(malletPosition.x > (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() && malletPosition.x < (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius())
			{
				malletPosition.y = _topPlayer->get_radius();
			}
			else
			{
				malletPosition.y = _topPlayer->get_radius() + _table_bottom_right->getContentSize().height;
			}
		}
	}

	if(player_id == 1)
	{
        //CCLog("mallet position y: %f", malletPosition.y);
		if(malletPosition.y < (_screenSize.height / 2) + _topPlayer->get_radius())
		{
			malletPosition.y = (_screenSize.height / 2) + _topPlayer->get_radius();
		}

		if(malletPosition.y > _screenSize.height - _table_bottom_right->getContentSize().height - _topPlayer->get_radius())
		{
            if(malletPosition.x > (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() && malletPosition.x < (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius())
			{
                if(malletPosition.y > _screenSize.height - _topPlayer->get_radius())
                {
                    malletPosition.y = _screenSize.height - _topPlayer->get_radius();
                }
			}
			else
			{
				malletPosition.y = _screenSize.height - _table_bottom_right->getContentSize().height - _topPlayer->get_radius();
			}
		}
	}

	return malletPosition;
}

/********************************************//**
 *  Detects the collision of circular object with the puck and changes the puck vector
 ***********************************************/

void HockeyScene::puckCollisionVector(CCPoint objectCenter, float objectRadius, CCPoint objectVector)
{
	if(pow(objectCenter.x - _puck->getPositionX(),2) + pow(objectCenter.y - _puck->getPositionY(), 2) < pow(objectRadius + _puck->get_radius(), 2))
	{
		if(_puck_ticks > 10) // avoid that effect is played more than once every collision
		{
				SimpleAudioEngine::sharedEngine()->playEffect("puck.wav");
				_puck_ticks = 0;
		}

		CCPoint current_puck_vector = _puck->getVector();

		float puck_vector_force = sqrt(pow(objectVector.x, 2) + pow(objectVector.y, 2) + pow(current_puck_vector.x, 2) + pow(current_puck_vector.y, 2)) * _friction;

		float puck_vector_angle = atan2(_puck->getPositionY() - objectCenter.y, _puck->getPositionX() - objectCenter.x);

		/**
		 *  check if object is a corner and dont let it overlap it
		 */

		if(objectCenter.y == 0 || objectCenter.y == _screenSize.height)
		{
			CCPoint new_puck_position = ccp(objectCenter.x + (sin(puck_vector_angle + PI) * (objectRadius + _puck->get_radius())), objectCenter.y + (cos(puck_vector_angle + PI) * (objectRadius + _puck->get_radius())));

			_puck->setPosition(new_puck_position);
		}

        CCPoint new_puck_vector = ccp(puck_vector_force * 0.7 * cos(puck_vector_angle), puck_vector_force * sin(puck_vector_angle));

		// multiply vector per vectors radio
		_puck->setVector(new_puck_vector);
	}
}

void HockeyScene::keyBackClicked()
{
	showPauseMenu();
}
