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

    struct ResolutionConfig
    {
        cocos2d::Size size;
        std::string dirname;
        float scaleFactor;
        int height;

        inline ResolutionConfig(const cocos2d::Size &size, const std::string &dirname, const float scaleFactor, const int height)
            : size(size), dirname(dirname), scaleFactor(scaleFactor), height(height) { }

        inline ResolutionConfig() : size(), dirname(""), scaleFactor(1.0f), height(0) { }

        inline void setConfig(const cocos2d::Size &size, const std::string &dirname, const float scaleFactor)
        {
            this->size = size;
            this->dirname = dirname;
            this->scaleFactor = scaleFactor;
        }
    };

    class MultiResolution
    {
        typedef std::vector<ResolutionConfig> ResolutionList;
    private:
        cocos2d::Size designSize;
        ResolutionPolicy policy;
        static ResolutionList resolutions;
        float realScale, scaleH, scaleW;
        ResolutionConfig currentConfig;

        cocos2d::Size swap(const cocos2d::Size &size, const bool doSwap);
        void initNames();
    public:
        inline static MultiResolution &getInstance()
        {
            static MultiResolution r;
            return r;
        }

        MultiResolution() : designSize(960, 640), policy(ResolutionPolicy::UNKNOWN) { initNames(); }
        ~MultiResolution() { }

        void resolve();
        void setDirectory(const ResolutionType res, const char *dirname, const cocos2d::Size &size, const float scaleFactor);
        void copyConfig(const ResolutionType from, const ResolutionType to);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::Size &size) { designSize = size; }
        inline void setResolutionPolicy(const ResolutionPolicy policy) { this->policy = policy; }
        inline ResolutionPolicy getResolutionPolicy() const { return policy; }
        inline float getRealScaleFactor() const { return realScale; }
        inline float getScaleWidth() const { return scaleW; }
        inline float getScaleHeight() const { return scaleH; }
        inline const ResolutionConfig &getCurrentResolutionConfig() const { return currentConfig; }
    };
}

#endif