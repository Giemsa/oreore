#include "Move.h"

namespace oreore
{
    using namespace cocos2d;

    Move *Move::create(const float duration, const Point &vec)
    {
        Move *action = new Move();
        if(action && action->initWithDuration(duration, vec))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return nullptr;
    }

    bool Move::initWithDuration(const float duration, const Point &vec)
    {
        if(ActionInterval::initWithDuration(duration))
        {
            this->vec = vec;
            return true;
        }

        return false;
    }

    void Move::update(float time)
    {
        getOriginalTarget()->setPosition(getOriginalTarget()->getPosition() + vec);
    }

    void Move::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
    }

    void Move::stop(void)
    {
        ActionInterval::stop();
    }

    Move *Move::reverse() const
    {
        CCASSERT(0, "reverse is not implemented");
        return nullptr;
    }

    Move *Move::clone() const
    {
        return Move::create(getDuration(), vec);
    }
}

