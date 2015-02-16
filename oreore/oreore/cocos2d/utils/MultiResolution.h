#ifndef __OREORE_COCOS2D_MULTIRESOLITION_H__
#define __OREORE_COCOS2D_MULTIRESOLITION_H__

#include "cocos2d.h"
#include <vector>

namespace oreore
{
    enum class ResolutionType
    {
        iPhone,
        iPhoneHD,
        iPad,
        iPhoneHD4inch,
        iPadHD,

        Small,
        Medium,
        Large,
        XLarge,

        All
    };

    class ResolutionConfig
    {
        friend class MultiResolution;
    private:
        cocos2d::Size size;
        cocos2d::Application::Platform platform;
        std::string dirname;
        bool platformSet;
        float scaleFactor;
        std::function<bool ()> selector;
    public:
        inline ResolutionConfig()
            : size(), dirname(""), scaleFactor(1.0f), selector(nullptr),
              platform(), platformSet(false)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const cocos2d::Size &size
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              size(size),
              selector(nullptr), platform(), platformSet(false)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const std::function<bool ()> &selector
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              selector(selector),
              size(), platform(), platformSet(false)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const cocos2d::Application::Platform platform
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              platform(platform),
              size(), selector(nullptr), platformSet(true)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const cocos2d::Application::Platform platform, const cocos2d::Size &size
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              platform(platform),size(size),
              selector(nullptr), platformSet(true)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const cocos2d::Size &size, const std::function<bool ()> &selector
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              size(size), selector(selector),
              platform(), platformSet(false)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const cocos2d::Application::Platform platform, const std::function<bool ()> &selector
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              platform(platform), selector(selector),
              size(), platformSet(true)
        { }

        inline ResolutionConfig(
            const std::string &dirname, const float scaleFactor,
            const cocos2d::Application::Platform platform,
            const cocos2d::Size &size, const std::function<bool ()> &selector
        )
            : dirname(dirname), scaleFactor(scaleFactor),
              platform(platform),
              size(size), selector(selector),
              platformSet(true)
        { }

        inline void setConfig(const cocos2d::Size &size, const std::string &dirname, const float scaleFactor)
        {
            this->size = size;
            this->dirname = dirname;
            this->scaleFactor = scaleFactor;
        }

        inline const cocos2d::Size &getSize() const { return size; }
        inline const std::string &getDirName() const { return dirname; }
        inline float getScaleFactor() const { return scaleFactor; }
        inline cocos2d::Application::Platform getPlatform() const { return platform; }
    };

    class MultiResolution final
    {
        using ResolutionList = std::vector<ResolutionConfig>;
    private:
        cocos2d::Size designSize;
        ResolutionPolicy policy;
        ResolutionList resolutions;
        ResolutionConfig currentConfig;
        float baseScaling;

        cocos2d::Size swap(const cocos2d::Size &size, const bool doSwap);
    public:
        inline static MultiResolution &getInstance()
        {
            static MultiResolution r;
            return r;
        }

        MultiResolution()
        : designSize(960, 640)
        , policy(ResolutionPolicy::UNKNOWN)
        , baseScaling(2.0f)
        { }
        ~MultiResolution() { }

        void resolve();
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::Size &size) { designSize = size; }
        inline void setResolutionPolicy(const ResolutionPolicy policy) { this->policy = policy; }
        inline ResolutionPolicy getResolutionPolicy() const { return policy; }
        inline const ResolutionConfig &getCurrentResolutionConfig() const { return currentConfig; }

        void setResolutionList(const std::initializer_list<ResolutionConfig> &args);
        inline float getBaseScaling() const { return baseScaling; }
        inline void setBaseScaling(const float scale) { baseScaling = scale; }
        void enableDebugAssetDirectory(const std::string &dir = "debug");
    };
}

#endif

