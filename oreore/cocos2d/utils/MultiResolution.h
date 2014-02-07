#ifndef __OREORE_COCOS2D_MULTIRESOLITION_H__
#define __OREORE_COCOS2D_MULTIRESOLITION_H__

#include "cocos2d.h"
#include <vector>

namespace oreore
{
    struct Resolution
    {
        cocos2d::Size size;
        char directory[64];

        Resolution(const cocos2d::Size &size, const char *name);
        bool operator<(const Resolution &rhs) const;
        bool operator>(const Resolution &rhs) const;
    };

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
        static const char *names[static_cast<int>(ResolutionType::All)];

        cocos2d::Size swap(const cocos2d::Size &size, const bool doSwap);
    public:
        MultiResolution();
        ~MultiResolution() { }

        void resolve();
        void addDirectory(const ResolutionType res, const char *name);
        void addSearchPath(const char *name);
    };
}

#endif
