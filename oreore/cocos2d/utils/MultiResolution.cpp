#include "MultiResolution.h"

namespace oreore
{
    using namespace cocos2d;

    /* MultiResolution */
    MultiResolution::ResolutionList MultiResolution::resolutions;

    void MultiResolution::initNames()
    {
        if(resolutions.size() > 0)
            return;

        resolutions.reserve(static_cast<int>(ResolutionType::All));

        /* iOS */
        resolutions.push_back(
            ResolutionConfig(
                Size(320, 480),
                "resources-iphone",
                0.5f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(640, 960),
                "resources-iphonehd",
                1.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(768, 1024),
                "resources-ipad",
                1.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(1536, 2048),
                "resources-ipadhd",
                2.0f
            )
        );

        /* Android */
        resolutions.push_back(
            ResolutionConfig(
                Size(360, 640),
                "resources-small",
                0.5f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(540, 960),
                "resources-medium",
                1.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(720, 1280),
                "resources-large",
                2.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(1080, 1920),
                "resources-xlarge",
                3.0f
            )
        );
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
        const int offset = (platform == ApplicationProtocol::Platform::OS_IPHONE || platform == ApplicationProtocol::Platform::OS_IPAD) ? 0 : 4;
        bool set = false;
        float factor = 1.0f;
        for(int i = 2 + offset; i >= offset; i--)
        {
            if(size.height > resolutions[i].size.height)
            {
                const ResolutionConfig &config = resolutions[i + 1];
                order.push_back(config.dirname);
                rsize = config.size;
                factor = config.scaleFactor;
                set = true;
                break;
            }
        }
        if(!set)
        {
            const ResolutionConfig &config = resolutions[offset];
            order.push_back(config.dirname);
            rsize = config.size;
            factor = config.scaleFactor;
        }

        Director::getInstance()->setContentScaleFactor(std::min(rsize.width / designSize.width, rsize.height / designSize.height));
        //Director::getInstance()->setContentScaleFactor(factor);
        eglView->setDesignResolutionSize(designSize.width, designSize.height, policy);
        FileUtils::getInstance()->setSearchResolutionsOrder(order);
    }

    void MultiResolution::setDirectory(const ResolutionType res, const char *dirname, const Size &size, const float scaleFactor)
    {
        if(res == ResolutionType::All)
        {
            for(int i = 0; i < static_cast<int>(ResolutionType::All); i++)
                resolutions[i].setConfig(size, dirname, scaleFactor);
            return;
        }
        resolutions[static_cast<int>(res)].setConfig(size, dirname, scaleFactor);
    }

    void MultiResolution::copyConfig(const ResolutionType from, const ResolutionType to)
    {
        resolutions[static_cast<int>(to)] = resolutions[static_cast<int>(from)];
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
            FileUtils::getInstance()->addSearchPath(name);
    }
}
