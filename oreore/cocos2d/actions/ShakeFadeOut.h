#ifndef __OREORE_COCOS2D_SHAKEFADEOUT_H__
#define __OREORE_COCOS2D_SHAKEFADEOUT_H__

#include "cocos2d.h"
#include "Shake.h"

namespace oreore
{
    class CCShakeFadeOut : public CCShake
    {
    public:
        static CCShakeFadeOut* create(const float duration, const float level);
        static CCShakeFadeOut* create(const float duration, const float level_x, const float level_y);

        virtual void update(float time);

        virtual CCShakeFadeOut *reverse(); // override
        virtual CCShakeFadeOut *clone();
    };
}

#endif
