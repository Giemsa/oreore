#include "MusicFadeTo.h"
#include "SimpleAudioEngine.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    MusicFadeTo *MusicFadeTo::create(const float duration, const float volume, const bool pauseOnComplete)
    {
        MusicFadeTo *action = new MusicFadeTo();
        if(action && action->initWithDuration(duration, volume, pauseOnComplete))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return nullptr;
    }

    bool MusicFadeTo::initWithDuration(const float duration, const float volume, const bool pauseOnComplete)
    {
        if(!ActionInterval::initWithDuration(duration))
        {
            return false;
        }

        pause = pauseOnComplete;
        this->volume = volume;

        return true;
    }

    void MusicFadeTo::update(float time)
    {
        const float vol = baseValue + (volume - baseValue) * time;
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(vol);
    }

    void MusicFadeTo::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        baseValue = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
    }

    void MusicFadeTo::stop(void)
    {
        if(pause)
        {
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        }

        ActionInterval::stop();
    }

    MusicFadeTo *MusicFadeTo::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return nullptr;
    }

    MusicFadeTo *MusicFadeTo::clone() const
    {
        return MusicFadeTo::create(getDuration(), volume, pause);
    }
}

