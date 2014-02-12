#include "MultiResolution.h"

namespace oreore
{
    using namespace cocos2d;

    /* MultiResolution */
    const char *MultiResolution::names[static_cast<int>(ResolutionType::All)] = {
        "resources-iphone",
        "resources-iphonehd",
        "resources-ipad",
        "resources-ipadhs",

        "resources-xlarge",
        "resources-large",
        "resources-medium",
        "resources-small"
    };

    Size MultiResolution::swap(const Size &size, const bool doSwap)
    {
        if(doSwap)
            return Size(size.height, size.width);
        return size;
    }

    void MultiResolution::resolve()
    {
        EGLView *eglView = EGLView::getInstance();
        Size rsize;
        const Size fsize = eglView->getFrameSize();
        const bool v = fsize.height / fsize.width > 1.0f;
        const Size size = swap(fsize, v);

        std::vector<std::string> order;

        ApplicationProtocol::Platform platform = Application::getInstance()->getTargetPlatform();
        if(platform == ApplicationProtocol::Platform::OS_IPHONE || platform == ApplicationProtocol::Platform::OS_IPAD)
        {
            if(size.height > 768)
                order.push_back(names[static_cast<int>(ResolutionType::iPadHD)]);
            else if(size.height > 640)
                order.push_back(names[static_cast<int>(ResolutionType::iPad)]);
            else if(size.height > 480)
                order.push_back(names[static_cast<int>(ResolutionType::iPhoneHD)]);
            else
                order.push_back(names[static_cast<int>(ResolutionType::iPhone)]);
        }
        else if(platform == ApplicationProtocol::Platform::OS_ANDROID)
        {
            if(size.height > 1200)
                order.push_back(names[static_cast<int>(ResolutionType::XLarge)]);
            else if(size.height > 960)
                order.push_back(names[static_cast<int>(ResolutionType::Large)]);
            else if(size.height > 480)
                order.push_back(names[static_cast<int>(ResolutionType::Medium)]);
            else
                order.push_back(names[static_cast<int>(ResolutionType::Small)]);
        }

        eglView->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
        FileUtils::getInstance()->setSearchPaths(searchPaths);
        FileUtils::getInstance()->setSearchResolutionsOrder(order);
    }

    void MultiResolution::addDirectory(const ResolutionType res, const char *name)
    {
        if(res == ResolutionType::All)
        {
            for(int i = 0; i < static_cast<int>(ResolutionType::All); i++)
                names[i] = name;
            return;
        }
        names[static_cast<int>(res)] = name;
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
            searchPaths.push_back(name);
    }
}
