#include "MultiResolution.h"
#include <vector>

namespace oreore
{
    using namespace cocos2d;

    /* Resolution */
    Resolution::Resolution(const Size &size, const char *name) : size(size)
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
    
    MultiResolution::MultiResolution()
    {

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
        const bool v = fsize.height / fsize.width > 1.0f;
        const Size size = swap(fsize, v);
        const Size dsize = swap(Size(960, 640), v);

        std::vector<std::string> order;

        ApplicationProtocol::Platform platform = Application::getInstance()->getTargetPlatform();
        if(platform == ApplicationProtocol::Platform::OS_IPHONE || platform == ApplicationProtocol::Platform::OS_IPAD)
        {
            if(size.height > 768)
            {
                rsize.setSize(2048, 1536);
                order.push_back(names[static_cast<int>(ResolutionType::iPadHD)]);
            }
            else if(size.height > 640)
            {
                rsize.setSize(1536, 768);
                order.push_back(names[static_cast<int>(ResolutionType::iPad)]);
            }
            else if(size.height > 480)
            {
                if(size.width < 1136)
                    rsize.setSize(960, 649);
                else
                    rsize.setSize(1136, 640);
                order.push_back(names[static_cast<int>(ResolutionType::iPhoneHD)]);
            }
            else
            {
                rsize.setSize(480, 320);
                order.push_back(names[static_cast<int>(ResolutionType::iPhone)]);
            }
        }
        else if(platform == ApplicationProtocol::Platform::OS_ANDROID)
        {
            if(size.height > 1200)
            {
                rsize.setSize(1200, 800);
                order.push_back(names[static_cast<int>(ResolutionType::XLarge)]);
            }
            else if(size.height > 960)
            {
                rsize.setSize(960, 640);
                order.push_back(names[static_cast<int>(ResolutionType::Large)]);
            }
            else if(size.height > 480)
            {
                rsize.setSize(720, 480);
                order.push_back(names[static_cast<int>(ResolutionType::Medium)]);
            }
            else
            {
                rsize.setSize(568, 320);
                order.push_back(names[static_cast<int>(ResolutionType::Small)]);
            }
        }

        rsize = swap(rsize, v);
        Director::getInstance()->setContentScaleFactor(dsize.width / rsize.width);
        eglView->setDesignResolutionSize(rsize.width, rsize.height,/* kResolutionFixedHeight */ ResolutionPolicy::SHOW_ALL);
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
