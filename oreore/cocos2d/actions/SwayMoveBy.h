#ifndef __OREORE_COCOS2D_SWAYMOVEBY_H__
#define __OREORE_COCOS2D_SWAYMOVEBY_H__

#include "cocos2d.h"

namespace oreore
{
    namespace SwayDirection
    {
        enum Type
        {
            Vertical,
            Horizontal
        };
    }

    class CCSwayMoveBy : public cocos2d::CCActionInterval
    {
    protected:
        cocos2d::CCPoint pos, dpos;
        float amp, freq;
        SwayDirection::Type dir;
    public:
        CCSwayMoveBy() { }
        virtual ~CCSwayMoveBy() { }

        inline static CCSwayMoveBy* create(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq)
        {
            return create(duration, pos, amp, freq, SwayDirection::Horizontal);
        }

        static CCSwayMoveBy* create(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type);
        bool initWithDuration(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type);

        virtual void startWithTarget(cocos2d::CCNode *pTarget); // override
        virtual void update(float time); // override
        virtual void stop(void); // override

        virtual CCSwayMoveBy *reverse(); // override
        virtual CCSwayMoveBy *clone(); // override
    };
}

#endif
