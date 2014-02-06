#include "Move.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCMove *CCMove::create(const float duration, const CCPoint &vec)
    {
        CCMove *action = new CCMove();
        if(action && action->initWithDuration(duration, vec))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCMove::initWithDuration(const float duration, const CCPoint &vec)
    {
        if(CCActionInterval::initWithDuration(duration))
        {
            this->vec = vec;
            return true;
        }

        return false;
    }

    void CCMove::update(float time)
    {
        getOriginalTarget()->setPosition(getOriginalTarget()->getPosition() + vec);
    }

    void CCMove::startWithTarget(CCNode *target)
    {
        CCActionInterval::startWithTarget(target);
    }

    void CCMove::stop(void)
    {
        CCActionInterval::stop();
    }

    CCMove *CCMove::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCMove *CCMove::clone()
    {
        return CCMove::create(getDuration(), vec);
    }
}
