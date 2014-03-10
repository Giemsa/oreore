#include "SimpleButton.h"

namespace oreore
{
    using namespace cocos2d;

    /* CCSimpleButton */
    CCSimpleButton* CCSimpleButton::createWithTexture(CCTexture2D *pTexture, CCObject* target, SEL_MenuHandler selector)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if (pobSprite && pobSprite->initWithTexture(pTexture, target, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::createWithTexture(CCTexture2D *pTexture, const CCRect& rect, CCObject* target, SEL_MenuHandler selector)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if (pobSprite && pobSprite->initWithTexture(pTexture, rect, target, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::create(const char *pszFileName, CCObject* target, SEL_MenuHandler selector)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if (pobSprite && pobSprite->initWithFile(pszFileName, target, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::create(const char *pszFileName, const CCRect& rect, CCObject* target, SEL_MenuHandler selector)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if (pobSprite && pobSprite->initWithFile(pszFileName, rect, target, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame, CCObject* target, SEL_MenuHandler selector)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame, target, selector))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::createWithSpriteFrameName(const char *pszSpriteFrameName, CCObject* target, SEL_MenuHandler selector)
    {
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);

    #if COCOS2D_DEBUG > 0
        char msg[256] = {0};
        sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
        CCAssert(pFrame != NULL, msg);
    #endif

        return createWithSpriteFrame(pFrame, target, selector);
    }

    CCSimpleButton* CCSimpleButton::create(CCObject* target, SEL_MenuHandler selector)
    {
        CCSimpleButton *pSprite = new CCSimpleButton();
        if (pSprite && pSprite->init(target, selector))
        {
            pSprite->autorelease();
            return pSprite;
        }
        CC_SAFE_DELETE(pSprite);
        return NULL;
    }

    void CCSimpleButton::_init(CCObject *target, SEL_MenuHandler selector)
    {
        this->target = target;
        this->selector = selector;
    }

    bool CCSimpleButton::init(CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::init();
    }

    bool CCSimpleButton::initWithTexture(CCTexture2D *pTexture, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithTexture(pTexture);
    }

    bool CCSimpleButton::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithTexture(pTexture, rect);
    }

    bool CCSimpleButton::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithTexture(pTexture, rect, rotated);
    }

    bool CCSimpleButton::initWithSpriteFrame(CCSpriteFrame *pSpriteFrame, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithSpriteFrame(pSpriteFrame);
    }

    bool CCSimpleButton::initWithSpriteFrameName(const char *pszSpriteFrameName, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithSpriteFrameName(pszSpriteFrameName);
    }

    bool CCSimpleButton::initWithFile(const char *pszFilename, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithFile(pszFilename);
    }

    bool CCSimpleButton::initWithFile(const char *pszFilename, const CCRect& rect, CCObject* target, SEL_MenuHandler selector)
    {
        _init(target, selector);
        return CCSprite::initWithFile(pszFilename, rect);
    }

    bool CCSimpleButton::ccTouchBegan(CCTouch *touch, CCEvent *event)
    {
        if(!touchEnabled)
            return false;

        const CCPoint &p = getParent()->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(touch->getLocationInView()));
        if(boundingBox().containsPoint(p))
        {
            bpos = p;
            runAction(touchAction());
            return true;
        }
        return false;
    }

    void CCSimpleButton::ccTouchMoved(CCTouch *touch, CCEvent *event)
    {
    
    }

    void CCSimpleButton::ccTouchEnded(CCTouch *touch, CCEvent *event)
    {
        const CCPoint &p = getParent()->convertToNodeSpace(CCDirector::sharedDirector()->convertToGL(touch->getLocationInView()));
        if(boundingBox().containsPoint(p))
        {
            const float d = p.getDistance(bpos);
            if(d < 30.0f && selector)
                (target->*selector)(this);
            runAction(unTouchAction());
        }
    }

    CCAction *CCSimpleButton::touchAction()
    {
        return CCScaleTo::create(0.1f, 0.9f);
    }

    CCAction *CCSimpleButton::unTouchAction()
    {
        return CCScaleTo::create(0.1f, 1.0f);
    }

    void CCSimpleButton::setTouchEnabled(const bool enable)
    {
        touchEnabled = enable;
    }

    bool CCSimpleButton::getTouchEnabled() const
    {
        return touchEnabled;
    }

    void CCSimpleButton::onEnter()
    {
        CCSprite::onEnter();
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
    }

    void CCSimpleButton::onExit()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        CCSprite::onExit();
    }
}
