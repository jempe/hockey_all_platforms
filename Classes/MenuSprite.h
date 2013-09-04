#ifndef MENUSPRITE_H
#define MENUSPRITE_H
#include "cocos2d.h"

using namespace cocos2d;

class MenuSprite : public CCSprite {
public:
    MenuSprite(void);
    virtual ~MenuSprite(void);

    static MenuSprite* createWithText(const char * button_text);

};

#endif // MENUSPRITE_H
