#ifndef __OREORE_COCOS2D_SWAYMOVEBY_H__
#define __OREORE_COCOS2D_SWAYMOVEBY_H__

#include "cocos2d.h"

namespace oreore
{
    enum class SwayDirection
    {
        Vertical,
        Horizontal
    };

    class SwayMoveBy : public cocos2d::ActionInterval
    {
    protected:
        cocos2d::Point pos, dpos;
        float amp, freq;
        SwayDirection dir;
    public:
        SwayMoveBy() { }
        virtual ~SwayMoveBy() { }

        inline static SwayMoveBy* create(const float duration, const cocos2d::Point &pos, const float amp, const float freq)
        {
            return create(duration, pos, amp, freq, SwayDirection::Horizontal);
        }

        static SwayMoveBy* create(const float duration, const cocos2d::Point &pos, const float amp, const float freq, const SwayDirection type);
        bool initWithDuration(const float duration, const cocos2d::Point &pos, const float amp, const float freq, const SwayDirection type);

        virtual void startWithTarget(cocos2d::Node *pTarget) override;
        virtual void update(float time) override;
        virtual void stop() override;

        virtual SwayMoveBy *reverse() const override;
        virtual SwayMoveBy *clone() const override;
    };
}

#endif
