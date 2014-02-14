#ifndef __OREORE_COCOS2D_SHAKE_H__
#define __OREORE_COCOS2D_SHAKE_H__

#include "cocos2d.h"

namespace oreore
{
    class Shake : public cocos2d::ActionInterval
    {
    protected:
        cocos2d::Point dpos;
        float strength_x, strength_y;
    public:
        Shake();

        static Shake* create(const float duration, const float level);
        static Shake* create(const float duration, const float level_x, const float level_y);
        bool initWithDuration(const float duration, const float level_x, const float level_y);

        virtual void startWithTarget(cocos2d::Node *pTarget) override;
        virtual void update(float time) override;
        virtual void stop(void) override;

        virtual Shake *reverse() const override;
        virtual Shake *clone() const override;
    };
}

#endif
