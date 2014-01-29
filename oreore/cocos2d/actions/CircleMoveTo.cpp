#include "CircleMoveTo.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCCircleMoveTo *CCCircleMoveTo::create(const float duration, const CCPoint &center, const float deg)
    {
        CCCircleMoveTo *action = new CCCircleMoveTo();
        action->initWithDuration(duration, center, deg);
        action->autorelease();
        return action;
    }

    bool CCCircleMoveTo::initWithDuration(const float duration, const CCPoint &center, const float deg)
    {
        if(!CCCircleMoveBy::initWithDuration(duration, center, deg))
            return false;

        return true;
    }

    void CCCircleMoveTo::startWithTarget(CCNode *target)
    {
        rad -= (target->getPosition() - center).getAngle();
        CCCircleMoveBy::startWithTarget(target);
    }

    CCCircleMoveTo *CCCircleMoveTo::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCCircleMoveTo *CCCircleMoveTo::clone()
    {
        return CCCircleMoveTo::create(getDuration(), center, deg);
    }
}
