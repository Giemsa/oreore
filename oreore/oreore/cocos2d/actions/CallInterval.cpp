#include "CallInterval.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CallInterval *CallInterval::create(const float duration, const std::function<void (float)> &func)
    {
        CallInterval *action = new CallInterval();
        if(action && action->initWithDuration(duration, func))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CallInterval::initWithDuration(const float duration, const std::function<void (float)> &func)
    {
        if(!ActionInterval::initWithDuration(duration))
            return false;

        this->func = func;
        return true;
    }

    void CallInterval::update(float time)
    {
        func(time);
    }

    CallInterval *CallInterval::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CallInterval *CallInterval::clone() const
    {
        return CallInterval::create(getDuration(), func);
    }
}

