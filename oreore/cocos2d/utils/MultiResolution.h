#ifndef __OREORE_COCOS2D_MULTIRESOLITION_H__
#define __OREORE_COCOS2D_MULTIRESOLITION_H__

#include "cocos2d.h"
#include <set>

namespace oreore
{
    namespace ResolutionType
    {
        enum Type
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
    }

    struct ResolutionConfig
    {
        cocos2d::CCSize size;
        std::string dirname;
        float scaleFactor;
        int height;

        inline ResolutionConfig(const cocos2d::CCSize &size, const std::string &dirname, const float scaleFactor, const int height)
            : size(size), dirname(dirname), scaleFactor(scaleFactor), height(height) { }

        inline void setConfig(const cocos2d::CCSize &size, const std::string &dirname, const float scaleFactor)
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
        cocos2d::CCSize designSize;
        ResolutionPolicy policy;
        static ResolutionList resolutions;

        cocos2d::CCSize swap(const cocos2d::CCSize &size, const bool doSwap);
        void initNames();
    public:
        MultiResolution() : designSize(960, 640), policy(kResolutionShowAll) { initNames(); }
        MultiResolution(const cocos2d::CCSize &designSize) : designSize(designSize), policy(kResolutionShowAll) { initNames(); }
        ~MultiResolution() { }

        void resolve();
        void setDirectory(const ResolutionType::Type res, const char *dirname, const cocos2d::CCSize &size, const float scaleFactor);
        void copyConfig(const ResolutionType::Type from, const ResolutionType::Type to);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::CCSize &size) { designSize = size; }
        inline void setResolutionPolicy(const ResolutionPolicy policy) { this->policy = policy; }
        inline ResolutionPolicy getResolutionPolicy() const { return policy; }
    };
}

#endif
