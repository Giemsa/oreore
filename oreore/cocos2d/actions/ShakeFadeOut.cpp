#include "ShakeFadeOut.h"
#include "../../Utils.h"
#include "../../null.h"

namespace oreore
{
    using namespace cocos2d;

    ShakeFadeOut *ShakeFadeOut::create(const float duration, const float strength)
    {
        return create(duration, strength, strength);
    }

    ShakeFadeOut *ShakeFadeOut::create(const float duration, const float level_x, const float level_y)
    {
        ShakeFadeOut *action = new ShakeFadeOut();
        if(action && action->initWithDuration(duration, level_x, level_y))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    void ShakeFadeOut::update(float time)
    {
        const float x = random<float>(-strength_x, strength_x) * (1.0f - time);
        const float y = random<float>(-strength_y, strength_y) * (1.0f - time);

        getOriginalTarget()->setPosition(dpos + Point(x, y));
    }

    ShakeFadeOut *ShakeFadeOut::reverse() const
    {
        return clone();
    }

    ShakeFadeOut *ShakeFadeOut::clone() const
    {
        return ShakeFadeOut::create(getDuration(), strength_x, strength_y);
    }
}
