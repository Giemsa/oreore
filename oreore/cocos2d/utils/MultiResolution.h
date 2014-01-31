#ifndef __OREORE_COCOS2D_MULTIRESOLITION_H__
#define __OREORE_COCOS2D_MULTIRESOLITION_H__

#include "cocos2d.h"
#include <set>

namespace oreore
{
	struct Resolution
	{
		cocos2d::CCSize size;
		char directory[64];

        Resolution(const cocos2d::CCSize &size, const char *name);
        bool operator<(const Resolution &rhs) const;
        bool operator>(const Resolution &rhs) const;
	};

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
        static const char *names[ResolutionType::All];
        //static const char *searchPaths[2];

        cocos2d::CCSize swap(const cocos2d::CCSize &size, const bool doSwap);
	public:
		MultiResolution();
		~MultiResolution() { }

        void resolve();
        void addDirectory(const ResolutionType::Type res, const char *name);
        void addSearchPath(const char *name);
	};
}

#endif
