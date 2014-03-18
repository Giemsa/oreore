#include "Sound.h"
#include "../../Utils.h"
#include "../actions/MusicFadeTo.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    /* SoundManager */
    std::unique_ptr<SoundManager> SoundManager::manager;

    bool SoundManager::init()
    {
        if(!CCNode::init())
            return false;

        bgmVolume = 1.0f;
        seVolume = 1.0f;
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
            SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), loop);
        else
        {
            setBGMVolume(0.0f);
            SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), loop);
            fadeIn(duration);
        }
    }

    void SoundManager::completeFadeOut()
    {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        currentlyPlaying.clear();
    }

    void SoundManager::stopBGM(const float duration)
    {
        if(duration == 0.0f)
            completeFadeOut();
        else
            runAction(
                CCSequence::create(
                    MusicFadeTo::create(duration, 0.0f, false),
                    CallFunc::create(CC_CALLBACK_0(SoundManager::completeFadeOut, this)),
                    NULL
                )
            );
    }

    unsigned int SoundManager::playSE(const std::string &filename, const bool loop)
    {
        if(!enabled || seMute)
            return 0;

        return SimpleAudioEngine::getInstance()->playEffect(filename.c_str(), loop);
    }

    void SoundManager::stopSE(const unsigned int id)
    {
        SimpleAudioEngine::getInstance()->stopEffect(id);
    }

    float SoundManager::getBGMVolume() const
    {
        return SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() / bgmVolume;
    }

    void SoundManager::setBGMVolume(const float volume)
    {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume * bgmVolume);
    }

    float SoundManager::getSEVolume() const
    {
        return SimpleAudioEngine::getInstance()->getEffectsVolume() / seVolume;
    }

    void SoundManager::setSEVolume(const float volume)
    {
        SimpleAudioEngine::getInstance()->setEffectsVolume(volume * seVolume);
    }

    void SoundManager::setMaxBGMVolume(const float volume)
    {
        bgmVolume = range(0.0f, volume, 1.0f);
        setBGMVolume(SimpleAudioEngine::getInstance()->getBackgroundMusicVolume());
    }

    void SoundManager::setMaxSEVolume(const float volume)
    {
        seVolume = range(0.0f, volume, 1.0f);
        setSEVolume(SimpleAudioEngine::getInstance()->getEffectsVolume());
    }

    void SoundManager::fadeIn(const float duration)
    {
        if(!enabled || bgmMute)
        {
            setBGMVolume(1.0f);
            return;
        }

        runAction(
            MusicFadeTo::create(duration, bgmVolume)
        );
    }

    void SoundManager::fadeOut(const float duration, const bool pauseOnComplete)
    {
        runAction(
            MusicFadeTo::create(duration, 0.0f, pauseOnComplete)
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
            MusicFadeTo::create(duration, volume * bgmVolume)
        );
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
                MusicFadeTo::create(duration / 2.0f, 0.0f),
                CallFunc::create([this]() {
                    setBGMVolume(0.0f);
                    playBGM(reservedMusicFile);
                    reservedMusicFile.clear();
                }),
                MusicFadeTo::create(duration / 2.0f, bgmVolume),
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
            if(!currentlyPlaying.empty())
                playBGM(currentlyPlaying);
            this->enabled = true;
        }
        else
        {
            SimpleAudioEngine::getInstance()->stopAllEffects();
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            this->enabled = false;
        }
    }

    void SoundManager::setBGMMute(const bool mute)
    {
        if(bgmMute == mute)
            return;

        if(mute)
        {
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            bgmMute = true;
        }
        else
        {
            bgmMute = false;
            if(!currentlyPlaying.empty())
                playBGM(currentlyPlaying);
        }
    }

    void SoundManager::setSEMute(const bool mute)
    {
        if(seMute == mute)
            return;

        seMute = mute;

        /*
        if(mute)
        {
            SimpleAudioEngine::sharedEngine()->stopAllEffects();
            seMute = true;
        }
        else
            seMute = false;
        */
    }
}
