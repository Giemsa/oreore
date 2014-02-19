#include "MultiResolution.h"
#include <vector>

namespace oreore
{
    using namespace cocos2d;

    /* MultiResolution */
    MultiResolution::ResolutionList MultiResolution::resolutions;

    void MultiResolution::initNames()
    {
        if(resolutions.size() > 0)
            return;

        resolutions.reserve(ResolutionType::All);

        /* iOS */
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(320, 480),
                "resources-iphone",
                0.5f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(640, 960),
                "resources-iphonehd",
                1.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(768, 1024),
                "resources-ipad",
                1.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(1536, 2048),
                "resources-ipadhd",
                2.0f
            )
        );

        /* Android */
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(360, 640),
                "resources-small",
                0.5f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(540, 960),
                "resources-medium",
                1.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(720, 1280),
                "resources-large",
                2.0f
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(1080, 1920),
                "resources-xlarge",
                3.0f
            )
        );
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
        const CCSize size = swap(fsize, fsize.height / fsize.width < 1.0f);

        std::vector<std::string> order;

        TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();

        const int offset = (platform == kTargetIphone || platform == kTargetIpad) ? 0 : 4;
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

        CCDirector::sharedDirector()->setContentScaleFactor(std::min(rsize.width / designSize.width, rsize.height / designSize.height));
        //CCDirector::sharedDirector()->setContentScaleFactor(factor);
        eglView->setDesignResolutionSize(designSize.width, designSize.height, policy);
        CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(order);
    }

    void MultiResolution::setDirectory(const ResolutionType::Type res, const char *dirname, const CCSize &size, const float scaleFactor)
    {
        if(res == ResolutionType::All)
        {
            for(int i = 0; i < ResolutionType::All; i++)
                resolutions[i].setConfig(size, dirname, scaleFactor);
            return;
        }
        resolutions[res].setConfig(size, dirname, scaleFactor);
    }

    void MultiResolution::copyConfig(const ResolutionType::Type from, const ResolutionType::Type to)
    {
        resolutions[to] = resolutions[from];
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
            CCFileUtils::sharedFileUtils()->addSearchPath(name);
    }
}
