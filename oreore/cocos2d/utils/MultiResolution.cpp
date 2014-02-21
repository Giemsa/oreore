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
                //Size(384, 568),
                "resources-iphone",
                0.5f,
                480
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(640, 960),
                //Size(768, 1136),
                "resources-iphonehd",
                1.0f,
                960
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //Size(768, 1024),
                Size(768, 1136),
                "resources-ipad",
                1.0f,
                1024
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //Size(640, 1136),
                Size(768, 1136),
                "resources-iphonehd",
                1.0f,
                1136
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                Size(1536, 2048),
                //Size(1536, 2272),
                "resources-ipadhd",
                2.0f,
                2048
            )
        );

        /* Android */
        resolutions.push_back(
            ResolutionConfig(
                //Size(360, 640),
                Size(384, 568),
                "resources-small",
                0.5f,
                640
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //Size(540, 960),
                Size(576, 852),
                "resources-medium",
                1.0f,
                960
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //Size(720, 1280),
                Size(768, 1136),
                "resources-large",
                2.0f,
                1280
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //Size(1080, 1920),
                Size(1536, 2272),
                "resources-xlarge",
                3.0f,
                1920
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

        int offset = 0;
        int len = 0;
        if(platform == ApplicationProtocol::Platform::OS_IPHONE || platform == ApplicationProtocol::Platform::OS_IPAD)
        {
            offset = 0;
            len = 3;
        }
        else
        {
            offset = 5;
            len = 7;
        }
        bool set = false;
        float factor = 1.0f;
        for(int i = len; i >= offset; i--)
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
        

        realScale = std::max(fsize.width / designSize.width, fsize.height / designSize.height);
        Director::getInstance()->setContentScaleFactor(std::min(rsize.width / designSize.width, rsize.height / designSize.height));
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
        resolutions[static_cast<int>(to)].size = resolutions[static_cast<int>(from)].size;
        resolutions[static_cast<int>(to)].dirname = resolutions[static_cast<int>(from)].dirname;
        resolutions[static_cast<int>(to)].scaleFactor = resolutions[static_cast<int>(from)].scaleFactor;
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
            FileUtils::getInstance()->addSearchPath(name);
    }
}
