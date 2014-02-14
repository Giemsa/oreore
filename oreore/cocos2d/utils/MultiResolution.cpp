#include "MultiResolution.h"

namespace oreore
{
    using namespace cocos2d;

    /* MultiResolution */
    MultiResolution::StringList MultiResolution::names;

    void MultiResolution::initNames()
    {
        if(names.size() > 0)
            return;

        names.reserve(static_cast<int>(ResolutionType::All));
        names.push_back("resources-iphone");
        names.push_back("resources-iphonehd");
        names.push_back("resources-ipad");
        names.push_back("resources-ipadhd");

        names.push_back("resources-small");
        names.push_back("resources-medium");
        names.push_back("resources-large");
        names.push_back("resources-xlarge");
    }

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
        const Size size = swap(fsize, fsize.height / fsize.width < 1.0f);

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

        eglView->setDesignResolutionSize(designSize.width, designSize.height, policy);
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
            FileUtils::getInstance()->addSearchPath(name);
    }
}
