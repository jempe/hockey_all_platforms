/*
 * VectorSprite.cpp
 *
 *  Created on: Jun 14, 2013
 *      Author: sucio
 */

#include "VectorSprite.h"

VectorSprite::VectorSprite() {
	_vector = ccp(0, 0);

}

VectorSprite::~VectorSprite() {
	// TODO Auto-generated destructor stub
}

VectorSprite* VectorSprite::vectorSpriteWithFile(const char * fileName)
{
	VectorSprite* sprite = new VectorSprite();

	if(sprite && sprite->initWithFile(fileName))
	{
		sprite->autorelease();
		return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return NULL;
}

void VectorSprite::setPosition(const CCPoint& pos)
{
	CCSprite::setPosition(pos);

	if( ! _nextPos.equals(pos))
	{
		_nextPos = pos;
	}
}

float VectorSprite::get_radius()
{
	return getTexture()->getContentSize().width * 0.43f;
}

