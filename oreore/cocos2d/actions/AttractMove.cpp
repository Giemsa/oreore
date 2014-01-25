#include "AttractMove.h"

namespace oreore
{
    using namespace cocos2d;

    AttractMove *AttractMove::create(const float duration, const Point &target, const Point &v0)
    {
        AttractMove *action = new AttractMove();
        action->initWithDuration(duration, target, v0);
        action->autorelease();
        return action;
    }

    bool AttractMove::initWithDuration(const float duration, const Point &target, const Point &v0)
    {
        if(ActionInterval::initWithDuration(duration))
        {
            this->target = target;
            this->v0 = v0;
            return true;
        }

        return false;
    }

    void AttractMove::update(float time)
    {
        const float t1 = getDuration() * time;
        getOriginalTarget()->setPosition(dpos + v0 * t1 + accelVec * (t1 * t1 / 2));
    }

    void AttractMove::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        dpos = target->getPosition();
        accelVec = ((this->target - dpos) - v0 * getDuration()) * 2 / (getDuration() * getDuration());
    }

    void AttractMove::stop(void)
    {
        ActionInterval::stop();
    }

    AttractMove *AttractMove::reverse() const
    {
        CCASSERT(0, "reverse is not implemented");
        return nullptr;
    }

    AttractMove *AttractMove::clone() const
    {
        return AttractMove::create(getDuration(), target, v0);
    }
}