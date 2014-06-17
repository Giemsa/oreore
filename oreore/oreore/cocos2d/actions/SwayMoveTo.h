#ifndef __OREORE_COCOS2D_SWAYMOVETO_H__
#define __OREORE_COCOS2D_SWAYMOVETO_H__

#include "cocos2d.h"
#include "SwayMoveBy.h"

namespace oreore
{
    class SwayMoveTo : public SwayMoveBy
    {
    public:
        SwayMoveTo() { }
        virtual ~SwayMoveTo() { }

        inline static SwayMoveTo* create(const float duration, const cocos2d::Point &pos, const float amp, const float freq)
        {
            return create(duration, pos, amp, freq, SwayDirection::Horizontal);
        }

        static SwayMoveTo* create(const float duration, const cocos2d::Point &pos, const float amp, const float freq, const SwayDirection type);
        bool initWithDuration(const float duration, const cocos2d::Point &pos, const float amp, const float freq, const SwayDirection type);

        virtual void startWithTarget(cocos2d::Node *pTarget) override;

        virtual SwayMoveTo *reverse() const override;
        virtual SwayMoveTo *clone() const override;
    };
}

#endif
