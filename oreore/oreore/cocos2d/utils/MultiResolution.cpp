#include "MultiResolution.h"

namespace oreore
{
    using namespace cocos2d;

    /* MultiResolution */
    Size MultiResolution::swap(const Size &size, const bool doSwap)
    {
        if(doSwap)
        {
            return Size(size.height, size.width);
        }
        return size;
    }

    void MultiResolution::resolve()
    {
        Director *dir = Director::getInstance();
        GLView *glView = dir->getOpenGLView();
        Size rsize;
        const Size fsize = glView->getFrameSize();
        const Size size = swap(fsize, fsize.height / fsize.width < 1.0f);

        std::vector<std::string> order;
        ApplicationProtocol::Platform platform = Application::getInstance()->getTargetPlatform();
        bool found = false;

        std::sort(resolutions.begin(), resolutions.end(), [](const ResolutionConfig &lhs, const ResolutionConfig &rhs) {
            return lhs.size.height > rhs.size.height;
        });

        for(ResolutionConfig &config : resolutions)
        {
            if(config.platformSet)
            {
                if(
                    (
                        (config.platform != Application::Platform::OS_IPHONE && config.platform != Application::Platform::OS_IPAD) ||
                        (platform != Application::Platform::OS_IPHONE && platform != Application::Platform::OS_IPAD)
                    ) && (
                        platform != config.platform
                    )
                )
                {
                    continue;
                }
            }

            if(config.selector)
            {
                if(!config.selector())
                {
                    continue;
                }
            }

            if(!config.size.equals(Size::ZERO))
            {
                if(size.height < config.size.height)
                {
                    continue;
                }
            }

            currentConfig = config;
            found = true;
            break;
        }

        CCASSERT(found, "resolution config not found");

        order.push_back(currentConfig.dirname);
        const float scaleH = size.height / designSize.height;
        const float scaleW = size.width / designSize.width;

        dir->setContentScaleFactor(currentConfig.scaleFactor / baseScaling);

        if(policy == ResolutionPolicy::UNKNOWN)
        {
            const ResolutionPolicy rp = scaleW > scaleH ? ResolutionPolicy::FIXED_HEIGHT : ResolutionPolicy::FIXED_WIDTH;
            glView->setDesignResolutionSize(designSize.width, designSize.height, rp);
        }
        else
        {
            glView->setDesignResolutionSize(designSize.width, designSize.height, policy);
        }

        FileUtils::getInstance()->setSearchResolutionsOrder(order);
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
        {
            FileUtils::getInstance()->addSearchPath(name);
        }
    }

    void MultiResolution::setResolutionList(const std::initializer_list<ResolutionConfig> &args)
    {
        for(auto &rc : args)
        {
            resolutions.push_back(rc);
        }
    }

    void MultiResolution::enableDebugAssetDirectory(const std::string &dir)
    {
        using PathList = std::vector<std::string>;

        const std::string &doc = FileUtils::getInstance()->getWritablePath();
        PathList paths;
        const PathList &searchPaths = FileUtils::getInstance()->getSearchPaths();
        paths.reserve(searchPaths.size() * 2);

        for(auto &path : searchPaths)
        {
            paths.push_back(doc + dir + "/" + path);
        }

        for(auto &path : searchPaths)
        {
            paths.push_back(path);
        }

        FileUtils::getInstance()->setSearchPaths(paths);
    }
}

