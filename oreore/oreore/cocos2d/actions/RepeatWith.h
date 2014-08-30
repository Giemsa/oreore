#ifndef __OREORE_COCOS2D_REPEATWITH_H__
#define __OREORE_COCOS2D_REPEATWITH_H__

#include "cocos2d.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    class RepeatWith : public cocos2d::ActionInterval
    {
    private:
        unsigned int times;
        unsigned int count;
        float span;
        std::function<void (float, unsigned int)> func;
    public:
        RepeatWith() { }
        virtual ~RepeatWith() { }

        static RepeatWith* create(
            const float duration,
            const unsigned int times,
            const std::function<void (float, unsigned int)> &func
        );
        virtual bool initWithDuration(
            const float duration,
            const unsigned int times,
            const std::function<void (float, unsigned int)> &func
        );

        virtual void update(float time) override;

        virtual RepeatWith *reverse() const override;
        virtual RepeatWith *clone() const override;
    };

    // fluxion plugin
    namespace fluxion
    {
        using RepeatWith = ActionInterval<RepeatWith>;
    }

    namespace x
    {
        inline fluxion::RepeatWith repeatWith(
            const float duration,
            const unsigned int times,
            const std::function<void (float, unsigned int)> &func
        )
        {
            return fluxion::RepeatWith(duration, times, func);
        }
    }
}

#endif

