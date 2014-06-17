#ifndef __OREORE_COCOS2D_ATTRACTMOVE_H__
#define __OREORE_COCOS2D_ATTRACTMOVE_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class AttractMove : public cocos2d::ActionInterval
    {
    private:
        cocos2d::Point dpos;
        cocos2d::Point target;
        cocos2d::Point accelVec, v0;

    public:
        AttractMove() { }
        virtual ~AttractMove() { }

        static AttractMove* create(const float duration, const cocos2d::Point &target, const cocos2d::Point &v0);
        bool initWithDuration(const float duration, const cocos2d::Point &target, const cocos2d::Point &v0);

        virtual void startWithTarget(cocos2d::Node *pTarget);
        virtual void update(float time);
        virtual void stop(void) override;

        virtual AttractMove *reverse() const override;
        virtual AttractMove *clone() const override;
    };
}

#endif
