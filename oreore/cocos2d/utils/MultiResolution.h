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

    class MultiResolution
    {
        typedef std::vector<std::string> StringList;
    private:
        cocos2d::CCSize designSize;
        ResolutionPolicy policy;
        static StringList names;

        cocos2d::CCSize swap(const cocos2d::CCSize &size, const bool doSwap);
        void initNames();
    public:
        MultiResolution() : designSize(960, 640), policy(kResolutionShowAll) { initNames(); }
        MultiResolution(const cocos2d::CCSize &designSize) : designSize(designSize), policy(kResolutionShowAll) { initNames(); }
        ~MultiResolution() { }

        void resolve();
        void setDirectory(const ResolutionType::Type res, const char *name);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::CCSize &size) { designSize = size; }
        inline void setResolutionPolicy(const ResolutionPolicy policy) { this->policy = policy; }
        inline ResolutionPolicy getResolutionPolicy() const { return policy; }
    };
}

#endif
