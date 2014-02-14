#include "MultiResolution.h"
#include <vector>

namespace oreore
{
    using namespace cocos2d;

    /* MultiResolution */
    MultiResolution::StringList MultiResolution::names;

    void MultiResolution::initNames()
    {
        if(names.size() > 0)
            return;

        names.reserve(ResolutionType::All);
        names.push_back("resources-iphone");
        names.push_back("resources-iphonehd");
        names.push_back("resources-ipad");
        names.push_back("resources-ipadhd");

        names.push_back("resources-small");
        names.push_back("resources-medium");
        names.push_back("resources-large");
        names.push_back("resources-xlarge");
    }

    CCSize MultiResolution::swap(const CCSize &size, const bool doSwap)
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
        const CCSize size = swap(fsize, fsize.height / fsize.width > 1.0f);

        std::vector<std::string> order;

        TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
        if(platform == kTargetIphone || platform == kTargetIpad)
        {
            if(size.height > 768)
                order.push_back(names[ResolutionType::iPadHD]);
            else if(size.height > 640)
                order.push_back(names[ResolutionType::iPad]);
            else if(size.height > 480)
                order.push_back(names[ResolutionType::iPhoneHD]);
            else
                order.push_back(names[ResolutionType::iPhone]);
        }
        else if(platform == kTargetAndroid)
        {
            if(size.height > 1200)
                order.push_back(names[ResolutionType::XLarge]);
            else if(size.height > 960)
                order.push_back(names[ResolutionType::Large]);
            else if(size.height > 480)
                order.push_back(names[ResolutionType::Medium]);
            else
                order.push_back(names[ResolutionType::Small]);
        }

        eglView->setDesignResolutionSize(designSize.width, designSize.height, policy);
        CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(order);
    }

    void MultiResolution::setDirectory(const ResolutionType::Type res, const char *name)
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
            CCFileUtils::sharedFileUtils()->addSearchPath(name);
    }
}
