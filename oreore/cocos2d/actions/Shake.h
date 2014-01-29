#ifndef __OREORE_COCOS2D_SHAKE_H__
#define __OREORE_COCOS2D_SHAKE_H__

#include "cocos2d.h"

namespace oreore
{
    class CCShake : public cocos2d::CCActionInterval
    {
    private:
        cocos2d::CCPoint dpos;
        float strength_x, strength_y;
    public:
        CCShake();

        static CCShake* create(const float duration, const float level);
        static CCShake* create(const float duration, const float level_x, const float level_y);
        bool initWithDuration(const float duration, const float level_x, const float level_y);

        virtual void startWithTarget(cocos2d::CCNode *pTarget);
        virtual void update(float time);
        virtual void stop(void) override;

        virtual CCShake *reverse(); // override
        virtual CCShake *clone();
    };
}

#endif
