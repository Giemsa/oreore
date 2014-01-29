#ifndef __OREORE_COCOS2D_ATTRACTMOVE_H__
#define __OREORE_COCOS2D_ATTRACTMOVE_H__

#include "cocos2d.h"

namespace oreore
{
    class CCAttractMove : public cocos2d::CCActionInterval
    {
    private:
        cocos2d::CCPoint dpos;
        cocos2d::CCPoint target;
        cocos2d::CCPoint accelVec, v0;

    public:
        CCAttractMove() { }
        virtual ~CCAttractMove() { }

        static CCAttractMove* create(const float duration, const cocos2d::CCPoint &target, const cocos2d::CCPoint &v0);
        bool initWithDuration(const float duration, const cocos2d::CCPoint &target, const cocos2d::CCPoint &v0);

        virtual void startWithTarget(cocos2d::CCNode *pTarget);
        virtual void update(float time);
        virtual void stop(void); // override

        virtual CCAttractMove *reverse(); // override
        virtual CCAttractMove *clone(); // override
    };
}

#endif
