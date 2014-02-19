#ifndef __OREORE_COCOS2D_UTILS_H__
#define __OREORE_COCOS2D_UTILS_H__

#include "cocos2d.h"

namespace oreore
{
    inline cocos2d::CCRect makeRect(const cocos2d::CCPoint &point, const cocos2d::CCSize &size)
    {
        return cocos2d::CCRect(point.x, point.y, size.width, size.height);
    }

    inline cocos2d::CCRect makeRect(const cocos2d::CCPoint &point, const int size)
    {
        return cocos2d::CCRect(point.x, point.y, size, size);
    }

    // Color4F -> Color3B
    inline cocos2d::ccColor3B toColor3B(const cocos2d::ccColor4F &color)
    {
        return cocos2d::ccc3(
            static_cast<GLubyte>(255.f * color.r),
            static_cast<GLubyte>(255.f * color.g),
            static_cast<GLubyte>(255.f * color.b)
        );
    }

    /* 色Aから色Bへのグラデーションを計算する(等比グラデーション) */
    template<
        int Ra, int Ga, int Ba,
        int Rb, int Gb, int Bb
    >
    inline cocos2d::ccColor3B gradient(const double count)
    {
        return cocos2d::ccc3(
            static_cast<GLubyte>((Rb - Ra) * count / 100 + Ra),
            static_cast<GLubyte>((Gb - Ga) * count / 100 + Ga),
            static_cast<GLubyte>((Ba - Ba) * count / 100 + Ba)
        );
    }

    inline cocos2d::ccColor3B gradient(const double count, const cocos2d::ccColor3B &a, const cocos2d::ccColor3B &b)
    {
        return cocos2d::ccc3(
            static_cast<GLubyte>((b.r - a.r) * count / 100 + a.r),
            static_cast<GLubyte>((b.g - a.g) * count / 100 + a.g),
            static_cast<GLubyte>((b.b - a.b) * count / 100 + a.b)
        );
    }

    inline cocos2d::CCPoint center()
    {
        const cocos2d::CCSize &s = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
        return cocos2d::CCPoint(s.width / 2, s.height / 2);
    }

    inline cocos2d::CCPoint center(cocos2d::CCNode *node)
    {
        const cocos2d::CCSize &s = node->getContentSize();
        return cocos2d::CCPoint(s.width / 2, s.height / 2);
    }

    inline cocos2d::CCPoint percent(const float x, const float y)
    {
        const cocos2d::CCSize &s = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
        return cocos2d::CCPoint(s.width * x / 100, s.height * y / 100);
    }

    inline cocos2d::CCPoint percent(cocos2d::CCNode *node, const float x, const float y)
    {
        const cocos2d::CCSize &s = node->getContentSize();
        return cocos2d::CCPoint(s.width * x / 100, s.height * y / 100);
    }

    inline int cut(const int v, const int n)
    {
        return v / n * n;
    }

    inline cocos2d::CCPoint cut(const cocos2d::CCPoint &p, const int n)
    {
        return cocos2d::CCPoint(cut(p.x, n), cut(p.y, n));
    }
}

#endif
