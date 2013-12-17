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
    
    // Color4F -> Color3B
    inline Color3B toColor3B(const cocos2d::Color4F &color)
    {
        return Color3B(
            static_cast<GLubyte>(255.f * color.r),
            static_cast<GLubyte>(255.f * color.g),
            static_cast<GLubyte>(255.f * color.b)
        );
    }
    
    inline cocos2d::Point center(cocos2d::Node *node)
    {
        const cocos2d::Size &s = node->getContentSize();
        return cocos2d::Point(s.width / 2, s.height / 2);
    }

    inline int cut(const int v, const int n)
    {
        return v / n * n;
    }

    inline cocos2d::Point cut(const cocos2d::Point &p, const int n)
    {
        return cocos2d::Point(cut(p.x, n), cut(p.y, n));
    }
}

#endif
