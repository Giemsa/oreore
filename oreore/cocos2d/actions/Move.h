#ifndef __OREORE_COCOS2D_MOVE_H__
#define __OREORE_COCOS2D_MOVE_H__

#include "cocos2d.h"

namespace oreore
{
    class CCMove : public cocos2d::CCActionInterval
    {
    private:
        cocos2d::CCPoint vec;
    public:
        CCMove() { }
        virtual ~CCMove() { }

        static CCMove* create(const float duration, const cocos2d::CCPoint &vec);
        bool initWithDuration(const float duration, const cocos2d::CCPoint &vec);

        virtual void startWithTarget(cocos2d::CCNode *pTarget);
        virtual void update(float time);
        virtual void stop(void); // override

        virtual CCMove *reverse(); // override
        virtual CCMove *clone();
    };
}

#endif
