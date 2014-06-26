#ifndef __OREORE_COCOS2D_FORMATTER_H__
#define __OREORE_COCOS2D_FORMATTER_H__

#include "cocos2d.h"

namespace oreore
{
    std::ostream& operator <<(std::ostream& os, const cocos2d::Point &point);
    std::ostream& operator <<(std::ostream& os, const cocos2d::Size &size);
    std::ostream& operator <<(std::ostream& os, const cocos2d::Rect &rect);

    inline std::ostream& operator <<(std::ostream& os, const cocos2d::Node *t)
    {
        return os << t->getDescription();
    }
}

#endif

