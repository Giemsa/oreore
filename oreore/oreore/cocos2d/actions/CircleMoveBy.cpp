#include "CircleMoveBy.h"
#include "../../Utils.h"

namespace oreore
{
    using namespace cocos2d;

    CircleMoveBy *CircleMoveBy::create(const float duration, const Point &center, const float deg)
    {
        CircleMoveBy *action = new CircleMoveBy();
        if(action && action->initWithDuration(duration, center, deg))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return nullptr;
    }

    bool CircleMoveBy::initWithDuration(const float duration, const Point &center, const float deg)
    {
        if(!ActionInterval::initWithDuration(duration))
        {
            return false;
        }

        this->center = center;
        this->deg = deg;
        rad = deg2rad(deg);

        return true;
    }

    void CircleMoveBy::update(float time)
    {
        const float r = rad * time;

        const float cr = cosf(r);
        const float sr = sinf(r);

        const float nx = (cr * cpos.x - sr * cpos.y) + center.x;
        const float ny = (sr * cpos.x + cr * cpos.y) + center.y;

        getOriginalTarget()->setPosition(Point(nx, ny));
    }

    void CircleMoveBy::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        pos = target->getPosition();
        cpos = pos - center;
    }

    void CircleMoveBy::stop(void)
    {
        ActionInterval::stop();
    }

    CircleMoveBy *CircleMoveBy::reverse() const
    {
        CCASSERT(0, "reverse is not implemented");
        return nullptr;
    }

    CircleMoveBy *CircleMoveBy::clone() const
    {
        return CircleMoveBy::create(getDuration(), center, deg);
    }
}

