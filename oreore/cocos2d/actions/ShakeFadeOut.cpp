#include "ShakeFadeOut.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    CCShakeFadeOut *CCShakeFadeOut::create(const float duration, const float strength)
    {
        return create(duration, strength, strength);
    }

    CCShakeFadeOut *CCShakeFadeOut::create(const float duration, const float level_x, const float level_y)
    {
        CCShakeFadeOut *action = new CCShakeFadeOut();
        if(action && action->initWithDuration(duration, level_x, level_y))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    void CCShakeFadeOut::update(float time)
    {
        const float x = random<float>(-strength_x, strength_x) * (1.0f - time);
        const float y = random<float>(-strength_y, strength_y) * (1.0f - time);

        getOriginalTarget()->setPosition(dpos + ccp(x, y));
    }

    CCShakeFadeOut *CCShakeFadeOut::reverse()
    {
        return clone();
    }

    CCShakeFadeOut *CCShakeFadeOut::clone()
    {
        return CCShakeFadeOut::create(getDuration(), strength_x, strength_y);
    }
}
