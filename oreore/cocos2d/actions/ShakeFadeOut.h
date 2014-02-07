#ifndef __OREORE_COCOS2D_SHAKEFADEOUT_H__
#define __OREORE_COCOS2D_SHAKEFADEOUT_H__

#include "cocos2d.h"
#include "Shake.h"

namespace oreore
{
    class ShakeFadeOut : public Shake
    {
    public:
        static ShakeFadeOut* create(const float duration, const float level);
        static ShakeFadeOut* create(const float duration, const float level_x, const float level_y);

        virtual void update(float time) override;

        virtual ShakeFadeOut *reverse() const override;
        virtual ShakeFadeOut *clone() const override;
    };
}

#endif
