#include "SwayMoveBy.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCSwayMoveBy *CCSwayMoveBy::create(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type)
    {
        CCSwayMoveBy *action = new CCSwayMoveBy();
        if(action && action->initWithDuration(duration, pos, amp, freq, type))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCSwayMoveBy::initWithDuration(const float duration, const cocos2d::CCPoint &pos, const float amp, const float freq, const SwayDirection::Type type)
    {
        if(!CCActionInterval::initWithDuration(duration))
            return false;

        this->pos = pos;
        this->amp = amp;
        this->freq = freq;
        dir = type;

        return true;
    }

    void CCSwayMoveBy::update(float time)
    {
        const CCPoint d = pos * time;

        const float s = amp * sinf(M_PI * 2 * freq * time);

        if(dir == SwayDirection::Horizontal)
            getOriginalTarget()->setPosition(dpos + ccp(d.x + s, d.y));
        else
            getOriginalTarget()->setPosition(dpos + ccp(d.x, d.y + s));
    }

    void CCSwayMoveBy::startWithTarget(CCNode *target)
    {
        CCActionInterval::startWithTarget(target);
        dpos = target->getPosition();
    }

    void CCSwayMoveBy::stop(void)
    {
        CCActionInterval::stop();
    }

    CCSwayMoveBy *CCSwayMoveBy::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCSwayMoveBy *CCSwayMoveBy::clone()
    {
        return CCSwayMoveBy::create(getDuration(), pos, amp, dir);
    }
}
