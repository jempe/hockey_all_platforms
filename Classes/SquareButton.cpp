#include "SquareButton.h"

SquareButton::SquareButton()
{

}

SquareButton::~SquareButton()
{
    // TODO Auto-generated destructor stub
}

SquareButton * SquareButton::createWithIcon(const char * icon_name, bool active)
{
    SquareButton* sprite = new SquareButton();

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    CCSpriteBatchNode * gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 1);

    if(sprite && sprite->initWithSpriteFrameName("square_button.png"))
    {
        sprite->autorelease();

        CCSprite * icon = CCSprite::createWithSpriteFrameName(icon_name);
        icon->setPosition(ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));
        sprite->addChild(icon);
        if( ! active)
        {
            icon->setColor(ccc3(170, 170, 170));
        }

        sprite->addChild(gameBatchNode);

        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return NULL;
}
