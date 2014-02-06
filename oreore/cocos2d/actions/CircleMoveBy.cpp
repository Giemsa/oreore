#include "CircleMoveBy.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCCircleMoveBy *CCCircleMoveBy::create(const float duration, const CCPoint &center, const float deg)
    {
        CCCircleMoveBy *action = new CCCircleMoveBy();
        if(action && action->initWithDuration(duration, center, deg))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCCircleMoveBy::initWithDuration(const float duration, const CCPoint &center, const float deg)
    {
        if(!CCActionInterval::initWithDuration(duration))
            return false;

        this->center = center;
        this->deg = deg;
        rad = deg2rad(deg);

        return true;
    }

    void CCCircleMoveBy::update(float time)
    {
        const float r = rad * time;

        const float cr = cosf(r);
        const float sr = sinf(r);

        const float nx = (cr * cpos.x - sr * cpos.y) + center.x;
        const float ny = (sr * cpos.x + cr * cpos.y) + center.y;

        getOriginalTarget()->setPosition(ccp(nx, ny));
    }

    void CCCircleMoveBy::startWithTarget(CCNode *target)
    {
        CCActionInterval::startWithTarget(target);
        pos = target->getPosition();
        cpos = pos - center;
    }

    void CCCircleMoveBy::stop(void)
    {
        CCActionInterval::stop();
    }

    CCCircleMoveBy *CCCircleMoveBy::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCCircleMoveBy *CCCircleMoveBy::clone()
    {
        return CCCircleMoveBy::create(getDuration(), center, deg);
    }
}
