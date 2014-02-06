#include "Shake.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCShake::CCShake() : strength_x(0), strength_y(0)
    {
    }

    CCShake *CCShake::create(const float duration, const float strength)
    {
        return create(duration, strength, strength);
    }

    CCShake *CCShake::create(const float duration, const float level_x, const float level_y)
    {
        CCShake *action = new CCShake();
        if(action && action->initWithDuration(duration, level_x, level_y))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCShake::initWithDuration(const float duration, const float level_x, const float level_y)
    {
        if(CCActionInterval::initWithDuration(duration))
        {
            strength_x = level_x;
            strength_y = level_y;
            return true;
        }

        return false;
    }

    void CCShake::update(float time)
    {
        const float x = random<float>(-strength_x, strength_x);
        const float y = random<float>(-strength_y, strength_y);

        getOriginalTarget()->setPosition(dpos + ccp(x, y));
    }

    void CCShake::startWithTarget(CCNode *target)
    {
        CCActionInterval::startWithTarget(target);
        dpos = target->getPosition();
    }

    void CCShake::stop(void)
    {
        getOriginalTarget()->setPosition(dpos);
        CCActionInterval::stop();
    }

    CCShake *CCShake::reverse()
    {
        return clone();
    }

    CCShake *CCShake::clone()
    {
        return CCShake::create(getDuration(), strength_x, strength_y);
    }
}
