#include "SimpleButton.h"

namespace oreore
{
    using namespace cocos2d;

    /* SimpleButton */
    SimpleButton* SimpleButton::createWithTexture(Texture2D *pTexture, const ccMenuCallback &selector)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithTexture(pTexture, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::createWithTexture(Texture2D *pTexture, const Rect& rect, const ccMenuCallback &selector)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithTexture(pTexture, rect, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::create(const char *pszFileName, const ccMenuCallback &selector)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithFile(pszFileName, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::create(const char *pszFileName, const Rect& rect, const ccMenuCallback &selector)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithFile(pszFileName, rect, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::createWithSpriteFrame(SpriteFrame *pSpriteFrame, const ccMenuCallback &selector)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::createWithSpriteFrameName(const char *pszSpriteFrameName, const ccMenuCallback &selector)
    {
        SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);

    #if COCOS2D_DEBUG > 0
        char msg[256] = {0};
        sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
        CCAssert(pFrame != NULL, msg);
    #endif

        return createWithSpriteFrame(pFrame, selector);
    }

    SimpleButton* SimpleButton::create(const ccMenuCallback &selector)
    {
        SimpleButton *pSprite = new SimpleButton();
        if (pSprite && pSprite->init(selector))
        {
            pSprite->autorelease();
            return pSprite;
        }
        CC_SAFE_DELETE(pSprite);
        return NULL;
    }


    void SimpleButton::_init(const ccMenuCallback &selector)
    {
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(SimpleButton::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(SimpleButton::onTouchEnded, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        this->selector = selector;
    }

    bool SimpleButton::init(const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::init();
    }

    bool SimpleButton::initWithTexture(Texture2D *pTexture, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithTexture(pTexture);
    }

    bool SimpleButton::initWithTexture(Texture2D *pTexture, const Rect& rect, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithTexture(pTexture, rect);
    }

    bool SimpleButton::initWithTexture(Texture2D *pTexture, const Rect& rect, bool rotated, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithTexture(pTexture, rect, rotated);
    }

    bool SimpleButton::initWithSpriteFrame(SpriteFrame *pSpriteFrame, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithSpriteFrame(pSpriteFrame);
    }

    bool SimpleButton::initWithSpriteFrameName(const char *pszSpriteFrameName, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithSpriteFrameName(pszSpriteFrameName);
    }

    bool SimpleButton::initWithFile(const char *pszFilename, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithFile(pszFilename);
    }

    bool SimpleButton::initWithFile(const char *pszFilename, const Rect& rect, const ccMenuCallback &selector)
    {
        _init(selector);
        return Sprite::initWithFile(pszFilename, rect);
    }

    bool SimpleButton::onTouchBegan(Touch *touch, Event *event)
    {
        const Point &p = getParent()->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
        if(boundingBox().containsPoint(p))
        {
            bpos = p;
            runAction(touchAction());
            return true;
        }
        return false;
    }

    void SimpleButton::onTouchEnded(Touch *touch, Event *event)
    {
        const Point &p = getParent()->convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
        if(boundingBox().containsPoint(p))
        {
            const float d = p.getDistance(bpos);
            if(d < 30.0f && selector)
                selector(this);
            runAction(unTouchAction());
        }
    }

    Action *SimpleButton::touchAction()
    {
        return ScaleTo::create(0.1f, 0.9f);
    }

    Action *SimpleButton::unTouchAction()
    {
        return CCScaleTo::create(0.1f, 1.0f);
    }
}
