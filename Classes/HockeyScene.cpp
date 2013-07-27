#include "HockeyScene.h"
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
	_gamePaused = false;
	_playersNumber = 1;
	_friction = 0.98;
	_bottomPlayerScore = 0;
	_topPlayerScore = 0;
	_computer_player_level = 1;

    //////////////////////////////
    // 1. super init first
    if ( ! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }

    _screenSize = CCDirector::sharedDirector()->getWinSize();

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

    // create score labels
    _top_player_score = CCLabelTTF::create("0", "Clubland", 30);
    _top_player_score->setPosition(ccp(_screenSize.width - 30, (_screenSize.height / 2) + 20));
    _top_player_score->setColor(ccc3(255, 0, 0));
    this->addChild(_top_player_score);

    _bottom_player_score = CCLabelTTF::create("0", "Clubland", 30);
    _bottom_player_score->setPosition(ccp(_screenSize.width - 30, (_screenSize.height / 2) - 20));
    _bottom_player_score->setColor(ccc3(255, 0, 0));
    this->addChild(_bottom_player_score);

    // create players mallets
    _topPlayer = VectorSprite::vectorSpriteWithFile("mallet.png");
    _topPlayer->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.75));
    this->addChild(_topPlayer);

    _bottomPlayer = VectorSprite::vectorSpriteWithFile("mallet.png");
    _bottomPlayer->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.25));
    this->addChild(_bottomPlayer);

    _computer_mallet_speed = _topPlayer->get_radius() / 18;

    _players = CCArray::create(_bottomPlayer, _topPlayer, NULL);
    _players->retain();


    // create puck
    _puck = VectorSprite::vectorSpriteWithFile("puck.png");
    _puck->setPosition(ccp(_screenSize.width / 2, _screenSize.height / 2));
    this->addChild(_puck);

    // listen for touches
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(HockeyScene::update));

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
						CCLog("player: %d", j);
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

	CCPoint puck_position = _puck->getPosition();
	VectorSprite * player;

	/**
	* detects collisions with mallets
	*/
	for(short int j = 0; j < _players->count(); j++)
	{
		player = (VectorSprite *) _players->objectAtIndex(j);

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
		puck_next_position.x = _table_left->getContentSize().width + _puck->get_radius();

		if(current_puck_vector.x < 0)
		{
			_puck->setVector(ccp(- current_puck_vector.x, current_puck_vector.y));
		}
	}
	else if(puck_next_position.x > _screenSize.width - _table_left->getContentSize().width - _puck->get_radius())
	{
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
		_puck->setVector(ccp(-current_puck_vector.x, current_puck_vector.y));
	}

	/**
	* detect and handle collisions with bottom and top of the table
	*/

	if(puck_next_position.x < (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() - _table_bottom_right->getContentSize().height || puck_next_position.x > (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius() + _table_bottom_right->getContentSize().height)
	{
		if(puck_next_position.y < _table_bottom_right->getContentSize().height + _puck->get_radius())
		{
			puck_next_position.y = _table_bottom_right->getContentSize().height + _puck->get_radius();

			if(current_puck_vector.y < 0)
			{
				_puck->setVector(ccp(current_puck_vector.x, -current_puck_vector.y));
			}
		}
		else if(puck_next_position.y > _screenSize.height -_table_bottom_right->getContentSize().height - _puck->get_radius())
		{
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
	else
	{
		CCPoint next_computer_mallet_position = computerMalletPosition();

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
/********************************************//**
 *  AI for computer mallet
 ***********************************************/
CCPoint HockeyScene::computerMalletPosition()
{
	CCPoint mallet_position = _topPlayer->getPosition();

	if(_puck->getPositionY() > _screenSize.height / 2)
	{
		if(pow(mallet_position.x - _puck->getPositionX(),2) + pow(mallet_position.y - _puck->getPositionY(), 2) < pow(_topPlayer->get_radius() + _puck->get_radius(), 2))
		{

		}
		else
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
	}
	else
	{
		float return_speed = 15;

		mallet_position.x += ((_screenSize.width / 2) - mallet_position.x) / return_speed;
		mallet_position.y += ((_screenSize.height * 0.75) - mallet_position.y) / return_speed;
	}

	mallet_position = keepMalletInsideCourt(1, mallet_position);

	return mallet_position;
}

/********************************************//**
 *  Update player score
 *  @param player int of the player
 *  	0 -> bottom player
 *  	1 -> top player
 ***********************************************/

void HockeyScene::playerScore(short int player)
{
	CCPoint center;

	if(player > 1)
	{
		_topPlayerScore++;
		center = ccp(_screenSize.width / 2, _screenSize.height / 2 + _puck->get_radius());
	}
	else
	{
		_bottomPlayerScore++;
		center = ccp(_screenSize.width / 2, _screenSize.height / 2 - _puck->get_radius());
	}

	/**
	 *  display score
	 */

	char score[10];

	sprintf(score,"%d", _topPlayerScore);
	_top_player_score->setString(score);

	sprintf(score,"%d", _bottomPlayerScore);
	_bottom_player_score->setString(score);

	CCLog("top: %d bottom: %d", _topPlayerScore, _bottomPlayerScore);

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
	CC_SAFE_RELEASE(_players);
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
			if(malletPosition.y < _topPlayer->get_radius() && malletPosition.x > (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() && malletPosition.x < (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius())
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
		if(malletPosition.y < (_screenSize.height / 2) + _topPlayer->get_radius())
		{
			malletPosition.y = (_screenSize.height / 2) + _topPlayer->get_radius();
		}

		if(malletPosition.y > _screenSize.height - _table_bottom_right->getContentSize().height - _topPlayer->get_radius())
		{
			if(malletPosition.y > _screenSize.height - _topPlayer->get_radius() && malletPosition.x > (_screenSize.width / 2) - (_center_circle->getContentSize().width / 2) + _puck->get_radius() && malletPosition.x < (_screenSize.width / 2) + (_center_circle->getContentSize().width / 2) - _puck->get_radius())
			{
				malletPosition.y = _screenSize.height - _topPlayer->get_radius();
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

		CCPoint new_puck_vector = ccp(puck_vector_force * cos(puck_vector_angle), puck_vector_force * sin(puck_vector_angle));

		// multiply vector per vectors radio
		_puck->setVector(new_puck_vector);
	}
}
