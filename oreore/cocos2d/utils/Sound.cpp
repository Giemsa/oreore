#include "Sound.h"
#include "../../Utils.h"
#include "../actions/MusicFadeTo.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    /* SoundManager */
    std::auto_ptr<SoundManager> SoundManager::manager;

    bool SoundManager::init()
    {
        if(!CCNode::init())
            return false;

        storedBGMVolume = bgmVolume = 1.0f;
        storedSEVolume = seVolume = 1.0f;
        enabled = true;
        reservedMusicFile.clear();
        currentlyPlaying.clear();

        return true;
    }

    SoundManager &SoundManager::getInstance()
    {
        if(!manager.get())
        {
            manager.reset(SoundManager::create());
            manager->retain();
            manager->onEnter();
        }
        return *manager;
    }

    void SoundManager::playBGM(const std::string &filename , const float duration, const bool loop)
    {
        if(currentlyPlaying == filename && SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            return;
        currentlyPlaying = filename;

        if(!enabled || bgmMute)
            return;

        if(duration == 0.0f)
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filename.c_str(), loop);
        else
        {
            setBGMVolume(0.0f);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filename.c_str(), loop);
            fadeIn(duration);
        }
    }

    void SoundManager::completeFadeOut()
    {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        currentlyPlaying.clear();
    }

    void SoundManager::stopBGM(const float duration)
    {
        if(duration == 0.0f)
            completeFadeOut();
        else
            runAction(
                CCSequence::create(
                    CCMusicFadeTo::create(duration, 0.0f, false),
                    CCCallFunc::create(this, callfunc_selector(SoundManager::completeFadeOut)),
                    NULL
                )
            );
    }

    unsigned int SoundManager::playSE(const std::string &filename, const bool loop)
    {
        if(!enabled || seMute)
            return 0;

        return SimpleAudioEngine::sharedEngine()->playEffect(filename.c_str(), loop);
    }

    void SoundManager::stopSE(const unsigned int id)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(id);
    }

    float SoundManager::getBGMVolume() const
    {
        if(!enabled || bgmMute)
            return storedBGMVolume;
        return SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() / bgmVolume;
    }

    void SoundManager::setBGMVolume(const float volume)
    {
        storedBGMVolume = volume * bgmVolume;
        if(enabled && !bgmMute)
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume * bgmVolume);
    }

    float SoundManager::getSEVolume() const
    {
        if(!enabled || seMute)
            return storedSEVolume;
        return SimpleAudioEngine::sharedEngine()->getEffectsVolume() / seVolume;
    }

    void SoundManager::setSEVolume(const float volume)
    {
        storedSEVolume = volume * seVolume;
        if(enabled && !seMute)
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume * seVolume);
    }

    void SoundManager::setMaxBGMVolume(const float volume)
    {
        bgmVolume = range(0.0f, volume, 1.0f);
        setBGMVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
    }

    void SoundManager::setMaxSEVolume(const float volume)
    {
        seVolume = range(0.0f, volume, 1.0f);
        setSEVolume(SimpleAudioEngine::sharedEngine()->getEffectsVolume());
    }

    void SoundManager::fadeIn(const float duration)
    {
        if(!enabled || bgmMute)
        {
            setBGMVolume(1.0f);
            return;
        }

        runAction(
            CCMusicFadeTo::create(duration, bgmVolume)
        );
    }

    void SoundManager::fadeOut(const float duration, const bool pauseOnComplete)
    {
        runAction(
            CCMusicFadeTo::create(duration, 0.0f, pauseOnComplete)
        );
    }

    void SoundManager::fadeTo(const float volume, const float duration)
    {
        if(!enabled || bgmMute)
        {
            setBGMVolume(volume);
            return;
        }

        runAction(
            CCMusicFadeTo::create(duration, volume * bgmVolume)
        );
    }

    void SoundManager::completeFading()
    {
        setBGMVolume(0.0f);
        playBGM(reservedMusicFile);
        reservedMusicFile.clear();
    }

    bool SoundManager::playWithFading(const std::string &filename, const float duration)
    {
        if(!enabled || bgmMute)
        {
            currentlyPlaying = filename;
            setBGMVolume(1.0f);
            return true;
        }

        if(!reservedMusicFile.empty())
        {
            CCAssert(reservedMusicFile.empty(), "reservedMusicFile is not empty.");
            return false;
        }
        reservedMusicFile = filename;
        runAction(
            CCSequence::create(
                CCMusicFadeTo::create(duration / 2.0f, 0.0f),
                CCCallFunc::create(this, callfunc_selector(SoundManager::completeFading)),
                CCMusicFadeTo::create(duration / 2.0f, bgmVolume),
                NULL
            )
        );

        return true;
    }

    void SoundManager::setEnabled(const bool enabled)
    {
        if(this->enabled == enabled)
            return;

        if(enabled)
        {
            setBGMVolume(storedBGMVolume);
            setSEVolume(storedSEVolume);
            if(!currentlyPlaying.empty())
                playBGM(currentlyPlaying);
            this->enabled = true;
        }
        else
        {
            SimpleAudioEngine::sharedEngine()->stopAllEffects();
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            storedBGMVolume = getBGMVolume();
            storedSEVolume = getSEVolume();
            this->enabled = false;
        }
    }

    void SoundManager::setBGMMute(const bool mute)
    {
        if(bgmMute == mute)
            return;

        if(mute)
        {
            storedBGMVolume = getBGMVolume();
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
            bgmMute = true;
        }
        else
        {
            bgmMute = false;
            setBGMVolume(storedBGMVolume);
            if(!currentlyPlaying.empty())
                playBGM(currentlyPlaying);
        }
    }

    void SoundManager::setSEMute(const bool mute)
    {
        if(seMute == mute)
            return;

        if(mute)
        {
            storedSEVolume = getSEVolume();
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
            seMute = true;
        }
        else
        {
            seMute = false;
            setSEVolume(storedSEVolume);
        }
    }
}
