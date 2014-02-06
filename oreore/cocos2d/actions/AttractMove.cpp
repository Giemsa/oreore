#include "AttractMove.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCAttractMove *CCAttractMove::create(const float duration, const CCPoint &target, const CCPoint &v0)
    {
        CCAttractMove *action = new CCAttractMove();
        if(action && action->initWithDuration(duration, target, v0))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCAttractMove::initWithDuration(const float duration, const CCPoint &target, const CCPoint &v0)
    {
        if(CCActionInterval::initWithDuration(duration))
        {
            this->target = target;
            this->v0 = v0;
            return true;
        }

        return false;
    }

    void CCAttractMove::update(float time)
    {
        const float t1 = getDuration() * time;
        getOriginalTarget()->setPosition(dpos + v0 * t1 + accelVec * (t1 * t1 / 2));
    }

    void CCAttractMove::startWithTarget(CCNode *target)
    {
        CCActionInterval::startWithTarget(target);
        dpos = target->getPosition();
        accelVec = ((this->target - dpos) - v0 * getDuration()) * 2 / (getDuration() * getDuration());
    }

    void CCAttractMove::stop(void)
    {
        CCActionInterval::stop();
    }

    CCAttractMove *CCAttractMove::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCAttractMove *CCAttractMove::clone()
    {
        return CCAttractMove::create(getDuration(), target, v0);
    }
}
