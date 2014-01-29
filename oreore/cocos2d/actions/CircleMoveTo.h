#ifndef __OREORE_COCOS2D_CIRCLEMOVETO_H__
#define __OREORE_COCOS2D_CIRCLEMOVETO_H__

#include "cocos2d.h"
#include "CircleMoveBy.h"

namespace oreore
{
    class CCCircleMoveTo : public CCCircleMoveBy
    {
    private:
    public:
        CCCircleMoveTo() { }
        virtual ~CCCircleMoveTo() { }

        static CCCircleMoveTo* create(const float duration, const cocos2d::CCPoint &center, const float deg);
        bool initWithDuration(const float duration, const cocos2d::CCPoint &center, const float deg);

        virtual void startWithTarget(cocos2d::CCNode *pTarget); // override

        virtual CCCircleMoveTo *reverse(); // override
        virtual CCCircleMoveTo *clone(); // override
    };
}

#endif
