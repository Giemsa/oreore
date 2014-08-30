#include "RepeatWith.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    RepeatWith *RepeatWith::create(
        const float duration,
        const unsigned int times,
        const std::function<void (float, unsigned int)> &func
    )
    {
        RepeatWith *action = new RepeatWith();
        if(action && action->initWithDuration(duration, times, func))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool RepeatWith::initWithDuration(
        const float duration,
        const unsigned int times,
        const std::function<void (float, unsigned int)> &func
    )
    {
        if(!ActionInterval::initWithDuration(duration))
            return false;

        this->func = func;
        this->times = times;
        count = 0;
        span = 1.0f / times;
        return true;
    }

    void RepeatWith::update(float time)
    {
        if(time > span * count)
        {
            func(time, count);
            ++count;
        }
    }

    RepeatWith *RepeatWith::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    RepeatWith *RepeatWith::clone() const
    {
        return RepeatWith::create(getDuration(), times, func);
    }
}

