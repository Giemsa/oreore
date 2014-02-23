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
                0.5f,
                480
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(640, 960),
                "resources-iphonehd",
                1.0f,
                960
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(768, 1024),
                "resources-ipad",
                1.0f,
                1024
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(640, 1136),
                "resources-iphonehd",
                1.0f,
                1136
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                CCSizeMake(1536, 2048),
                "resources-ipadhd",
                2.0f,
                2048
            )
        );

        /* Android */
        resolutions.push_back(
            ResolutionConfig(
                //CCSizeMake(360, 640),
                CCSizeMake(384, 568),
                "resources-small",
                0.5f,
                640
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //CCSizeMake(540, 960),
                CCSizeMake(576, 852),
                "resources-medium",
                1.0f,
                960
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //CCSizeMake(720, 1280),
                CCSizeMake(768, 1136),
                "resources-large",
                2.0f,
                1280
            )
        );
        resolutions.push_back(
            ResolutionConfig(
                //CCSizeMake(1080, 1920),
                CCSizeMake(1536, 2272),
                "resources-xlarge",
                3.0f,
                1920
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

        int offset = 0;
        int len = 0;
        if(platform == kTargetIpad || platform == kTargetIphone)
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
            if(size.height > resolutions[i].height)
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

        scaleH = rsize.height / designSize.height;
        scaleW = rsize.width / designSize.width;
        realScale = std::max(fsize.width / designSize.width, fsize.height / designSize.height);
        CCDirector::sharedDirector()->setContentScaleFactor(std::min(scaleW, scaleH));

        if(policy == kResolutionUnKnown)
        {
            const ResolutionPolicy rp = scaleW > scaleH ? kResolutionFixedHeight : kResolutionFixedWidth;
            eglView->setDesignResolutionSize(designSize.width, designSize.height, rp);
        }
        else
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
        resolutions[to].size = resolutions[from].size;
        resolutions[to].dirname = resolutions[from].dirname;
        resolutions[to].scaleFactor = resolutions[from].scaleFactor;
    }

    void MultiResolution::addSearchPath(const char *name)
    {
        if(name)
            CCFileUtils::sharedFileUtils()->addSearchPath(name);
    }
}
