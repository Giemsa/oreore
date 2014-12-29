#include "Sound.h"
#include "../../Utils.h"
#include "../actions/MusicFadeTo.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    /* SoundManager */
    std::unique_ptr<SoundManager> SoundManager::manager;

    namespace
    {
        std::string defaultResolver(const std::string &filename, const bool isBGM)
        {
            return filename;
        }
    }

    bool SoundManager::init()
    {
        if(!CCNode::init())
        {
            return false;
        }

        storedBGMVolume = bgmVolume = 1.0f;
        storedSEVolume = seVolume = 1.0f;
        enabled = true;
        reservedMusicFile.clear();
        currentlyPlaying.clear();

        resolver = defaultResolver;

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

    void SoundManager::playBGM(const std::string &filename, const float duration, const bool loop)
    {
        if(getBGMVolume() > 0.0f && currentlyPlaying == filename && SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
            return;
        }

        currentlyPlaying = filename;

        if(!enabled || bgmMute)
        {
            return;
        }

        if(duration == 0.0f)
        {
            SimpleAudioEngine::getInstance()->playBackgroundMusic(resolver(filename, true).c_str(), loop);
        }
        else
        {
            setBGMVolume(0.0f);
            SimpleAudioEngine::getInstance()->playBackgroundMusic(resolver(filename, true).c_str(), loop);
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
        {
            completeFadeOut();
        }
        else
        {
            runAction(
                CCSequence::create(
                    MusicFadeTo::create(duration, 0.0f, false),
                    CallFunc::create(CC_CALLBACK_0(SoundManager::completeFadeOut, this)),
                    NULL
                )
            );
        }
    }

    unsigned int SoundManager::playSE(const std::string &filename, const bool loop)
    {
        if(!enabled || seMute)
        {
            return 0;
        }

        return SimpleAudioEngine::getInstance()->playEffect(resolver(filename, false).c_str(), loop);
    }

    void SoundManager::stopSE(const unsigned int id)
    {
        SimpleAudioEngine::getInstance()->stopEffect(id);
    }

    float SoundManager::getBGMVolume() const
    {
        if(!enabled || bgmMute)
        {
            return storedBGMVolume;
        }

        return SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() / bgmVolume;
    }

    void SoundManager::setBGMVolume(const float volume)
    {
        storedBGMVolume = range(0.0f, volume, 1.0f) * bgmVolume;

        if(enabled && !bgmMute)
        {
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(storedBGMVolume);
        }
    }

    float SoundManager::getSEVolume() const
    {
        if(!enabled || seMute)
        {
            return storedSEVolume;
        }

        return SimpleAudioEngine::getInstance()->getEffectsVolume() / seVolume;
    }

    void SoundManager::setSEVolume(const float volume)
    {
        storedSEVolume = range(0.0f, volume, 1.0f) * seVolume;

        if(enabled && !seMute)
        {
            SimpleAudioEngine::getInstance()->setEffectsVolume(storedSEVolume);
        }
    }

    void SoundManager::setMaxBGMVolume(const float volume)
    {
        const float tmp = bgmVolume;
        bgmVolume = range(0.0f, volume, 1.0f);
        setBGMVolume(SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() / tmp);
    }

    void SoundManager::setMaxSEVolume(const float volume)
    {
        const float tmp = seVolume;
        seVolume = range(0.0f, volume, 1.0f);
        setSEVolume(SimpleAudioEngine::getInstance()->getEffectsVolume() / tmp);
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
        {
            return;
        }

        if(enabled)
        {
            setBGMVolume(storedBGMVolume);
            setSEVolume(storedSEVolume);
            if(!currentlyPlaying.empty())
            {
                playBGM(currentlyPlaying);
            }

            this->enabled = true;
        }
        else
        {
            SimpleAudioEngine::getInstance()->stopAllEffects();
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            storedBGMVolume = getBGMVolume();
            storedSEVolume = getSEVolume();
            this->enabled = false;
        }
    }

    void SoundManager::setBGMMute(const bool mute)
    {
        if(bgmMute == mute)
        {
            return;
        }

        if(mute)
        {
            storedBGMVolume = getBGMVolume();
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
            bgmMute = true;
        }
        else
        {
            bgmMute = false;
            setBGMVolume(storedBGMVolume);
            if(!currentlyPlaying.empty())
            {
                playBGM(currentlyPlaying);
            }
        }
    }

    void SoundManager::setSEMute(const bool mute)
    {
        if(seMute == mute)
        {
            return;
        }

        if(mute)
        {
            storedSEVolume = getSEVolume();
            SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
            seMute = true;
        }
        else
        {
            seMute = false;
            setSEVolume(storedSEVolume);
        }
    }

    template<typename T>
    void SoundManager::_preload(const T &list)
    {
        SimpleAudioEngine *eng = SimpleAudioEngine::getInstance();
        for(auto &file : list)
        {
            eng->preloadEffect(resolver(file, false).c_str());
        }
    }

    void SoundManager::preload(const std::initializer_list<std::string> &list) { _preload(list); }
    void SoundManager::preload(const FileList &list) { _preload(list); }

    template<typename T>
    void SoundManager::_unload(const T &list)
    {
        SimpleAudioEngine *eng = SimpleAudioEngine::getInstance();
        for(auto &file : list)
        {
            eng->unloadEffect(resolver(file, false).c_str());
        }
    }

    void SoundManager::unload(const std::initializer_list<std::string> &list) { _unload(list); }
    void SoundManager::unload(const FileList &list) { _unload(list); }
}
