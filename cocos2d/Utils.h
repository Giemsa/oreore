#ifndef __OREORE_COCOS2D_UTILS_H__
#define __OREORE_COCOS2D_UTILS_H__

#include "cocos2d.h"


#define Color3B cocos2d::Color3B

namespace Utils
{
	inline cocos2d::Rect makeRect(const cocos2d::Point &point, const cocos2d::Size &size)
	{
		return cocos2d::Rect(point.x, point.y, size.width, size.height);
	}

	inline cocos2d::Rect makeRect(const cocos2d::Point &point, const int size)
	{
		return cocos2d::Rect(point.x, point.y, size, size);
	}
}

#endif
