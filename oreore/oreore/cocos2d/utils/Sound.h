#ifndef __OREORE_COCOS2D_SOUND_H__
#define __OREORE_COCOS2D_SOUND_H__

#include <memory>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "../../null.h"

namespace oreore
{
    class SoundManager : public cocos2d::Node
    {
    private:
        bool enabled, bgmMute, seMute;
        float bgmVolume, seVolume, storedBGMVolume, storedSEVolume;
        std::string reservedMusicFile, currentlyPlaying;

        static std::unique_ptr<SoundManager> manager;
        CREATE_FUNC(SoundManager);

        void completeFading();
        void completeFadeOut();
    public:
        static SoundManager &getInstance();

        bool init(); // override

        void playBGM(const std::string &filename, const float duration = 0.0f, const bool loop = true);
        void stopBGM(const float duration = 0.0f);
        unsigned int playSE(const std::string &filename, const bool loop = false);
        void stopSE(const unsigned int id);

        float getBGMVolume() const;
        void setBGMVolume(const float volume);
        float getSEVolume() const;
        void setSEVolume(const float volume);

        inline float getMaxBGMVolume() const { return bgmVolume; }
        void setMaxBGMVolume(const float volume);
        inline float getMaxSEVolume() const { return seVolume; }
        void setMaxSEVolume(const float volume);

        void fadeIn(const float duration);
        void fadeOut(const float duration, const bool pauseOnComplete = false);
        void fadeTo(const float volume, const float duration);
        bool playWithFading(const std::string &filename, const float duration);

        void setEnabled(const bool enabled);
        inline bool isEnabled() const { return enabled; }
        void setBGMMute(const bool mute);
        inline bool isBGMMute() const { return bgmMute; }
        void setSEMute(const bool mute);
        inline bool isSEMute() const { return seMute; }
    };
}

#endif

