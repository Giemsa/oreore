#ifndef __OREORE_COCOS2D_MUSICFADETO_H__
#define __OREORE_COCOS2D_MUSICFADETO_H__

#include "cocos2d.h"

namespace oreore
{
    class CCMusicFadeTo : public cocos2d::CCActionInterval
    {
    private:
        bool pause;
        float baseValue, volume;
    public:
        CCMusicFadeTo() { }
        virtual ~CCMusicFadeTo() { }

        static CCMusicFadeTo* create(const float duration, const float volume, const bool pauseOnComplete = false);
        bool initWithDuration(const float duration, const float volume, const bool pauseOnComplete);

        virtual void startWithTarget(cocos2d::CCNode *taregt);
        virtual void update(float time);
        virtual void stop(void); // override

        virtual CCMusicFadeTo *reverse(); // override
        virtual CCMusicFadeTo *clone();
    };
}

#endif
