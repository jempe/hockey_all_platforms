/*
 * VectorSprite.h
 *
 *  Created on: Jun 14, 2013
 *      Author: sucio
 */

#ifndef VECTORSPRITE_H_
#define VECTORSPRITE_H_
#include "cocos2d.h"

using namespace cocos2d;

class VectorSprite : public CCSprite {
public:
	VectorSprite(void);
	virtual ~VectorSprite(void);

	virtual void setPosition(const CCPoint& pos);

	CC_SYNTHESIZE(CCPoint, _nextPos, NextPos);
	CC_SYNTHESIZE(CCPoint, _vector, Vector);
	CC_SYNTHESIZE(CCTouch*, _touch, Touch);

	static VectorSprite* vectorSpriteWithFile(const char* fileName);

	float get_radius();

};

#endif /* VECTORSPRITE_H_ */
