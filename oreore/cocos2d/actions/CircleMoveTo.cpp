#include "CircleMoveTo.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CircleMoveTo *CircleMoveTo::create(const float duration, const Point &center, const float deg)
    {
        CircleMoveTo *action = new CircleMoveTo();
        action->initWithDuration(duration, center, deg);
        action->autorelease();
        return action;
    }

    bool CircleMoveTo::initWithDuration(const float duration, const Point &center, const float deg)
    {
        if(!CircleMoveBy::initWithDuration(duration, center, deg))
            return false;

        return true;
    }

    void CircleMoveTo::startWithTarget(Node *target)
    {
        rad -= (target->getPosition() - center).getAngle();
        CircleMoveBy::startWithTarget(target);
    }

    CircleMoveTo *CircleMoveTo::reverse() const
    {
        CCASSERT(0, "reverse is not implemented");
        return null;
    }

    CircleMoveTo *CircleMoveTo::clone() const
    {
        return CircleMoveTo::create(getDuration(), center, deg);
    }
}
