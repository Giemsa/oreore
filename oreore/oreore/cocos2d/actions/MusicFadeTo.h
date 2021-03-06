#ifndef __OREORE_COCOS2D_MUSICFADETO_H__
#define __OREORE_COCOS2D_MUSICFADETO_H__

#include "cocos2d.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    class MusicFadeTo : public cocos2d::ActionInterval
    {
    private:
        bool pause;
        float baseValue, volume;
    public:
        MusicFadeTo() { }
        virtual ~MusicFadeTo() { }

        static MusicFadeTo* create(const float duration, const float volume, const bool pauseOnComplete = false);
        bool initWithDuration(const float duration, const float volume, const bool pauseOnComplete);

        virtual void startWithTarget(cocos2d::Node *taregt);
        virtual void update(float time);
        virtual void stop(void) override;

        virtual MusicFadeTo *reverse() const override;
        virtual MusicFadeTo *clone() const override;
    };

    // fluxion plugin
    namespace fluxion
    {
        using MusicFadeTo = ActionInterval<oreore::MusicFadeTo>;
    }

    namespace x
    {
        inline fluxion::MusicFadeTo musicFadeTo(
            const float duration, const float volume, const bool pauseOnComplete = false
        )
        {
            return fluxion::MusicFadeTo(duration, volume, pauseOnComplete);
        }
    }
}

#endif

