#include "Sound.h"
#include "../../Utils.h"
#include "MusicFadeTo.h"

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

        bgmVolume = 1.0f;
        seVolume = 1.0f;
        reservedMusicFile.clear();

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
        if(duration == 0.0f)
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filename.c_str(), loop);
        else
        {
            setBGMVolume(0.0f);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filename.c_str(), loop);
            fadeIn(duration);
        }
    }

    void SoundManager::stopBGM(const float duration)
    {
        if(duration == 0.0f)
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        else
            fadeOut(duration);
    }

    unsigned int SoundManager::playSE(const std::string &filename)
    {
        return SimpleAudioEngine::sharedEngine()->playEffect(filename.c_str(), false);
    }

    void SoundManager::stopSE(const unsigned int id)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(id);
    }

    float SoundManager::getBGMVolume() const
    {
        return SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() / bgmVolume;
    }

    void SoundManager::setBGMVolume(const float volume)
    {
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume * bgmVolume);
    }

    float SoundManager::getSEVolume() const
    {
        return SimpleAudioEngine::sharedEngine()->getEffectsVolume() / seVolume;
    }

    void SoundManager::setSEVolume(const float volume)
    {
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
}
