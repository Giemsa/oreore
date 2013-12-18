#include "Shake.h"
#include "../Utils.h"

namespace oreore
{
    using namespace cocos2d;

    Shake::Shake() : strength_x(0), strength_y(0)
    {
    }

    Shake *Shake::create(const float duration, const float strength)
    {
        return create(duration, strength, strength);
    }

    Shake *Shake::create(const float duration, const float level_x, const float level_y)
    {
        Shake *action = new Shake();
        action->initWithDuration(duration, level_x, level_y);
        action->autorelease();

        return action;
    }

    bool Shake::initWithDuration(const float duration, const float level_x, const float level_y)
    {
        if(ActionInterval::initWithDuration(duration))
        {
            strength_x = level_x;
            strength_y = level_y;
            return true;
        }

        return false;
    }

    void Shake::update(float time)
    {
        const float x = random<float>(-strength_x, strength_x);
        const float y = random<float>(-strength_y, strength_y);

        getOriginalTarget()->setPosition(dpos + Point(x, y));
    }

    void Shake::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        dpos = target->getPosition();
    }

    void Shake::stop(void)
    {
        getOriginalTarget()->setPosition(dpos);
        ActionInterval::stop();
    }

    Shake *Shake::reverse() const
    {
        return clone();
    }

    Shake *Shake::clone() const
    {
        return Shake::create(getDuration(), strength_x, strength_y);
    }
}
