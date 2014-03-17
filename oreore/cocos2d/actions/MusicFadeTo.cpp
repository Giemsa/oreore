#include "MusicFadeTo.h"
#include "../../null.h"
#include "SimpleAudioEngine.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    CCMusicFadeTo *CCMusicFadeTo::create(const float duration, const float volume, const bool pauseOnComplete)
    {
        CCMusicFadeTo *action = new CCMusicFadeTo();
        if(action && action->initWithDuration(duration, volume, pauseOnComplete))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCMusicFadeTo::initWithDuration(const float duration, const float volume, const bool pauseOnComplete)
    {
        if(!CCActionInterval::initWithDuration(duration))
            return false;

        pause = pauseOnComplete;
        this->volume = volume;

        return true;
    }

    void CCMusicFadeTo::update(float time)
    {
        const float vol = baseValue + (volume - baseValue) * time;
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(vol);
    }

    void CCMusicFadeTo::startWithTarget(CCNode *target)
    {
        CCActionInterval::startWithTarget(target);
        baseValue = SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
    }

    void CCMusicFadeTo::stop(void)
    {
        if(pause)
            SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        CCActionInterval::stop();
    }

    CCMusicFadeTo *CCMusicFadeTo::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCMusicFadeTo *CCMusicFadeTo::clone()
    {
        return CCMusicFadeTo::create(getDuration(), volume, pause);
    }
}
