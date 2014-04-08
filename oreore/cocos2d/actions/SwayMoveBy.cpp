#include "SwayMoveBy.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    SwayMoveBy *SwayMoveBy::create(const float duration, const Point &pos, const float amp, const float freq, const SwayDirection type)
    {
        SwayMoveBy *action = new SwayMoveBy();
        if(action && action->initWithDuration(duration, pos, amp, freq, type))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool SwayMoveBy::initWithDuration(const float duration, const Point &pos, const float amp, const float freq, const SwayDirection type)
    {
        if(!ActionInterval::initWithDuration(duration))
            return false;

        this->pos = pos;
        this->amp = amp;
        this->freq = freq;
        dir = type;

        return true;
    }

    void SwayMoveBy::update(float time)
    {
        const Point d = pos * time;

        const float s = amp * sinf(M_PI * 2 * freq * time);

        if(dir == SwayDirection::Horizontal)
            getOriginalTarget()->setPosition(dpos + Point(d.x + s, d.y));
        else
            getOriginalTarget()->setPosition(dpos + Point(d.x, d.y + s));
    }

    void SwayMoveBy::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        dpos = target->getPosition();
    }

    void SwayMoveBy::stop()
    {
        ActionInterval::stop();
    }

    SwayMoveBy *SwayMoveBy::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    SwayMoveBy *SwayMoveBy::clone() const
    {
        return SwayMoveBy::create(getDuration(), pos, amp, freq, dir);
    }
}
