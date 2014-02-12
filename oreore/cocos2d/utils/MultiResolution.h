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
        StringList searchPaths;
        cocos2d::CCSize designSize;
        static const char *names[ResolutionType::All];

        cocos2d::CCSize swap(const cocos2d::CCSize &size, const bool doSwap);
    public:
        MultiResolution() : designSize(960, 640) { }
        MultiResolution(const cocos2d::CCSize &designSize) : designSize(designSize) { }
        ~MultiResolution() { }

        void resolve();
        void addDirectory(const ResolutionType::Type res, const char *name);
        void addSearchPath(const char *name);
        inline void setDesignSize(const cocos2d::CCSize &size) { designSize = size; }
    };
}

#endif
