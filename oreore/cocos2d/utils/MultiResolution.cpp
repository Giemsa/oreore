#include "MultiResolution.h"
#include <vector>

namespace oreore
{
    using namespace cocos2d;

    /* Resolution */
    Resolution::Resolution(const CCSize &size, const char *name) : size(size)
    {
        strncpy(directory, name, sizeof(directory));
    }

    bool Resolution::operator<(const Resolution &rhs) const
    {
        return size.width == rhs.size.width ? size.height < rhs.size.width : size.width < rhs.size.width;
    }

    bool Resolution::operator>(const Resolution &rhs) const
    {
        return size.width == rhs.size.width ? size.height > rhs.size.width : size.width > rhs.size.width;
    }

    /* MultiResolution */
    const char *MultiResolution::names[ResolutionType::All] = {
        "resources-iphone",
        "resources-iphonehd",
        "resources-ipad",
        "resources-ipadhs",

        "resources-xlarge",
        "resources-large",
        "resources-medium",
        "resources-small"
    };
    
    enum
    {
        SEARCHPATH_IOS,
        SEARCHPATH_ANDROID
    };
   
    /*
    const char *MultiResolution::searchPaths[2] = {
        "Published-ios",
        ""
    };
    */
    
    MultiResolution::MultiResolution()
    {

    }

    cocos2d::CCSize MultiResolution::swap(const cocos2d::CCSize &size, const bool doSwap)
    {
        if(doSwap)
            return CCSizeMake(size.height, size.width);
        return size;
    }

    void MultiResolution::resolve()
    {
        CCEGLView *eglView = CCEGLView::sharedOpenGLView();
        CCSize rsize;
        const CCSize fsize = eglView->getFrameSize();
        const bool v = fsize.height / fsize.width > 1.0f;
        const CCSize size = swap(fsize, v);
        const CCSize dsize = swap(CCSizeMake(960, 640), v);

        std::vector<std::string> order;

        TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
        if(platform == kTargetIphone || platform == kTargetIpad)
        {
            if(size.height > 768)
            {
                rsize = CCSizeMake(2048, 1536);
                order.push_back(names[ResolutionType::iPadHD]);
            }
            else if(size.height > 640)
            {
                rsize = CCSizeMake(1536, 768);
                order.push_back(names[ResolutionType::iPad]);
            }
            else if(size.height > 480)
            {
                if(size.width < 1136)
                    rsize = CCSizeMake(960, 640);
                else
                    rsize = CCSizeMake(1136, 640);
                order.push_back(names[ResolutionType::iPhoneHD]);
            }
            else
            {
                rsize = CCSizeMake(480, 320);
                order.push_back(names[ResolutionType::iPhone]);
            }
        }
        else if(platform == kTargetAndroid)
        {
            if(size.height > 1200)
            {
                rsize = CCSizeMake(1200, 800);
                order.push_back(names[ResolutionType::XLarge]);
            }
            else if(size.height > 960)
            {
                rsize = CCSizeMake(960, 640);
                order.push_back(names[ResolutionType::Large]);
            }
            else if(size.height > 480)
            {
                rsize = CCSizeMake(720, 480);
                order.push_back(names[ResolutionType::Medium]);
            }
            else
            {
                rsize = CCSizeMake(568, 320);
                order.push_back(names[ResolutionType::Small]);
            }
        }

        rsize = swap(rsize, v);
        CCDirector::sharedDirector()->setContentScaleFactor(dsize.width / rsize.width);
        eglView->setDesignResolutionSize(rsize.width, rsize.height,/* kResolutionFixedHeight */ kResolutionShowAll);
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(order);
    }

    void MultiResolution::addDirectory(const ResolutionType::Type res, const char *name)
    {
        if(res == ResolutionType::All)
        {
            for(int i = 0; i < ResolutionType::All; i++)
                names[i] = name;
            return;
        }
        names[res] = name;
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
            searchPaths.push_back(name);
    }
}
