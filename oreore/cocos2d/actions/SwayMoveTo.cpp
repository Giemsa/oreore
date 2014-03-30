#include "SwayMoveTo.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCSwayMoveTo *CCSwayMoveTo::create(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type)
    {
        CCSwayMoveTo *action = new CCSwayMoveTo();
        if(action && action->initWithDuration(duration, pos, amp, freq, type))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCSwayMoveTo::initWithDuration(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type)
    {
        if(!CCActionInterval::initWithDuration(duration))
            return false;

        this->pos = pos;
        this->amp = amp;
        this->freq = freq;
        dir = type;

        return true;
    }

    void CCSwayMoveTo::startWithTarget(CCNode *target)
    {
        CCSwayMoveBy::startWithTarget(target);
        pos = pos - dpos;
    }

    CCSwayMoveTo *CCSwayMoveTo::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCSwayMoveTo *CCSwayMoveTo::clone()
    {
        return CCSwayMoveTo::create(getDuration(), pos, amp, dir);
    }
}
