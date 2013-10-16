#ifndef SQUAREBUTTON_H
#define SQUAREBUTTON_H
#include "cocos2d.h"

using namespace cocos2d;

class SquareButton : public CCSprite {
public:
    SquareButton(void);
    virtual ~SquareButton(void);

    static SquareButton* createWithIcon(const char * icon_name, bool active);

};

#endif // SQUAREBUTTON_H
