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
        StringList searchPaths;
        cocos2d::Size designSize;
        static const char *names[static_cast<int>(ResolutionType::All)];

        cocos2d::Size swap(const cocos2d::Size &size, const bool doSwap);
    public:
        MultiResolution() : designSize() { }
        MultiResolution(const cocos2d::Size &designSize) : designSize(designSize) { }
        ~MultiResolution() { }

        void resolve();
        void addDirectory(const ResolutionType res, const char *name);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::Size &size) { designSize = size; }
    };
}

#endif
