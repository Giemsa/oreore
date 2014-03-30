#ifndef __OREORE_COCOS2D_SWAYMOVETO_H__
#define __OREORE_COCOS2D_SWAYMOVETO_H__

#include "cocos2d.h"
#include "SwayMoveBy.h"

namespace oreore
{
    class CCSwayMoveTo : public CCSwayMoveBy
    {
    public:
        CCSwayMoveTo() { }
        virtual ~CCSwayMoveTo() { }

        inline static CCSwayMoveTo* create(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq)
        {
            return create(duration, pos, amp, freq, SwayDirection::Horizontal);
        }

        static CCSwayMoveTo* create(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type);
        bool initWithDuration(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type);

        virtual void startWithTarget(cocos2d::CCNode *pTarget); // override

        virtual CCSwayMoveTo *reverse(); // override
        virtual CCSwayMoveTo *clone(); // override
    };
}

#endif
