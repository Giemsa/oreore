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

        inline ResolutionConfig(const cocos2d::Size &size, const std::string &dirname, const float scaleFactor)
            : size(size), dirname(dirname), scaleFactor(scaleFactor) { }

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

        cocos2d::Size swap(const cocos2d::Size &size, const bool doSwap);
        void initNames();
    public:
        MultiResolution() : designSize(960, 640), policy(ResolutionPolicy::SHOW_ALL) { initNames(); }
        MultiResolution(const cocos2d::Size &designSize) : designSize(designSize), policy(ResolutionPolicy::SHOW_ALL) { initNames(); }
        ~MultiResolution() { }

        void resolve();
        void setDirectory(const ResolutionType res, const char *dirname, const cocos2d::Size &size, const float scaleFactor);
        void copyConfig(const ResolutionType from, const ResolutionType to);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::Size &size) { designSize = size; }
        inline void setResolutionPolicy(const ResolutionPolicy policy) { this->policy = policy; }
        inline ResolutionPolicy getResolutionPolicy() const { return policy; }
    };
}

#endif
