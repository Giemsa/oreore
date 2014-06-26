#include "SwayMoveTo.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    SwayMoveTo *SwayMoveTo::create(const float duration, const Point &pos, const float amp, const float freq, const SwayDirection type)
    {
        SwayMoveTo *action = new SwayMoveTo();
        if(action && action->initWithDuration(duration, pos, amp, freq, type))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool SwayMoveTo::initWithDuration(const float duration, const Point &pos, const float amp, const float freq, const SwayDirection type)
    {
        if(!ActionInterval::initWithDuration(duration))
            return false;

        this->pos = pos;
        this->amp = amp;
        this->freq = freq;
        dir = type;

        return true;
    }

    void SwayMoveTo::startWithTarget(Node *target)
    {
        SwayMoveBy::startWithTarget(target);
        pos = pos - dpos;
    }

    SwayMoveTo *SwayMoveTo::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    SwayMoveTo *SwayMoveTo::clone() const
    {
        return SwayMoveTo::create(getDuration(), pos, amp, freq, dir);
    }
}

