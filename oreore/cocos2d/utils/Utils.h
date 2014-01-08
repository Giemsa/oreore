#ifndef __OREORE_COCOS2D_UTILS_H__
#define __OREORE_COCOS2D_UTILS_H__

#include "cocos2d.h"

namespace oreore
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
    inline cocos2d::Color3B toColor3B(const cocos2d::Color4F &color)
    {
        return cocos2d::Color3B(
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
    inline cocos2d::Color3B gradient(const double count)
    {
        return cocos2d::Color3B(
            static_cast<GLubyte>((Rb - Ra) * count / 100 + Ra),
            static_cast<GLubyte>((Gb - Ga) * count / 100 + Ga),
            static_cast<GLubyte>((Ba - Ba) * count / 100 + Ba)
        );
    }

    inline cocos2d::Color3B gradient(const double count, const cocos2d::Color3B &a, const cocos2d::Color3B &b)
    {
        return cocos2d::Color3B(
            static_cast<GLubyte>((b.r - a.r) * count / 100 + a.r),
            static_cast<GLubyte>((b.g - a.g) * count / 100 + a.g),
            static_cast<GLubyte>((b.b - a.b) * count / 100 + a.b)
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
