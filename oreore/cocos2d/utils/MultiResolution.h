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

    class MultiResolution
    {
        typedef std::vector<std::string> StringList;
    private:
        cocos2d::Size designSize;
        ResolutionPolicy policy;
        static StringList names;

        cocos2d::Size swap(const cocos2d::Size &size, const bool doSwap);
        void initNames();
    public:
        MultiResolution() : designSize(960, 640), policy(ResolutionPolicy::SHOW_ALL) { initNames(); }
        MultiResolution(const cocos2d::Size &designSize) : designSize(designSize), policy(ResolutionPolicy::SHOW_ALL) { initNames(); }
        ~MultiResolution() { }

        void resolve();
        void addDirectory(const ResolutionType res, const char *name);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::Size &size) { designSize = size; }
        inline void setResolutionPolicy(const ResolutionPolicy policy) { this->policy = policy; }
        inline ResolutionPolicy getResolutionPolicy() const { return policy; }
    };
}

#endif
