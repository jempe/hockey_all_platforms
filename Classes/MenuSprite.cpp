#include "MenuSprite.h"

MenuSprite::MenuSprite()
{

}

MenuSprite::~MenuSprite()
{
    // TODO Auto-generated destructor stub
}

MenuSprite * MenuSprite::createWithText(const char * button_text, bool active)
{
    MenuSprite* sprite = new MenuSprite();

    if(sprite && sprite->initWithFile("players_button.png"))
    {
        sprite->autorelease();

        CCLabelBMFont * label = CCLabelBMFont::create(button_text, "londrina_solid.fnt");
        label->setPosition(ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height * 0.62));
        sprite->addChild(label);
        if(active)
        {
            CCLog("active button: %s", button_text);
            label->setColor(ccc3(255, 0, 0));
        }

        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return NULL;
}
