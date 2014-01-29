#ifndef __OREORE_COCOS2D_CIRCLEMOVEBY_H__
#define __OREORE_COCOS2D_CIRCLEMOVEBY_H__

#include "cocos2d.h"

namespace oreore
{
    class CCCircleMoveBy : public cocos2d::CCActionInterval
    {
    protected:
        cocos2d::CCPoint center, pos, cpos;
        float deg, rad;
    public:
        CCCircleMoveBy() { }
        virtual ~CCCircleMoveBy() { }

        static CCCircleMoveBy* create(const float duration, const cocos2d::CCPoint &center, const float deg);
        bool initWithDuration(const float duration, const cocos2d::CCPoint &center, const float deg);

        virtual void startWithTarget(cocos2d::CCNode *pTarget); // override
        virtual void update(float time); // override
        virtual void stop(void); // override

        virtual CCCircleMoveBy *reverse(); // override
        virtual CCCircleMoveBy *clone(); // override
    };
}

#endif
