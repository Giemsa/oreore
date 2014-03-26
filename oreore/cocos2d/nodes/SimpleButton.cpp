#include "SimpleButton.h"

namespace oreore
{
    using namespace cocos2d;

    /* CCSimpleButton */
    CCSimpleButton* CCSimpleButton::createWithTexture(CCTexture2D *pTexture)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if(pobSprite && pobSprite->initWithTexture(pTexture))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if(pobSprite && pobSprite->initWithTexture(pTexture, rect))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::create(const char *pszFileName)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if(pobSprite && pobSprite->initWithFile(pszFileName))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
    {
        CCSimpleButton *pobSprite = new CCSimpleButton();
        if(pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    CCSimpleButton* CCSimpleButton::createWithSpriteFrameName(const char *pszSpriteFrameName)
    {
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);

    #if COCOS2D_DEBUG > 0
        char msg[256] = {0};
        sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
        CCAssert(pFrame != NULL, msg);
    #endif

        return createWithSpriteFrame(pFrame);
    }

    void CCSimpleButton::_init()
    {
        this->target = null;
        this->selector = null;
        touchEnabled = true;
        priority = 0;
        touched = false;
        allowContinuousHit = false;
    }

    bool CCSimpleButton::init()
    {
        _init();
        return CCSprite::init();
    }

    bool CCSimpleButton::initWithTexture(CCTexture2D *pTexture)
    {
        _init();
        return CCSprite::initWithTexture(pTexture);
    }

    bool CCSimpleButton::initWithTexture(CCTexture2D *pTexture, const CCRect& rect)
    {
        _init();
        return CCSprite::initWithTexture(pTexture, rect);
    }

    bool CCSimpleButton::initWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
    {
        _init();
        return CCSprite::initWithSpriteFrame(pSpriteFrame);
    }

    bool CCSimpleButton::initWithSpriteFrameName(const char *pszSpriteFrameName)
    {
        _init();
        return CCSprite::initWithSpriteFrameName(pszSpriteFrameName);
    }

    bool CCSimpleButton::initWithFile(const char *pszFilename)
    {
        _init();
        return CCSprite::initWithFile(pszFilename);
    }

    bool CCSimpleButton::initWithFile(const char *pszFilename, const CCRect& rect)
    {
        _init();
        return CCSprite::initWithFile(pszFilename, rect);
    }

    void CCSimpleButton::endTouching()
    {
        touched = false;
    }

    bool CCSimpleButton::ccTouchBegan(CCTouch *touch, CCEvent *event)
    {
        if(!touchEnabled)
            return false;

        if(!allowContinuousHit && touched)
            return false;

        const CCPoint &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(boundingBox().containsPoint(p))
        {
            touched = true;
            bpos = p;
            CCFiniteTimeAction *action = touchAction();
            if(action)
                runAction(action);
            return true;
        }
        return false;
    }

    void CCSimpleButton::ccTouchMoved(CCTouch *touch, CCEvent *event)
    {
    
    }

    void CCSimpleButton::ccTouchEnded(CCTouch *touch, CCEvent *event)
    {
        const CCPoint &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(boundingBox().containsPoint(p))
        {
            const float d = p.getDistance(bpos);
            if(d < 30.0f && selector)
            {
                CCFiniteTimeAction *action = unTouchAction();
                if(action)
                    runAction(
                        CCSequence::create(
                            action,
                            CCCallFunc::create(this, callfunc_selector(CCSimpleButton::callCallback)),
                            NULL
                        )
                    );
                return;
            }
        }

        CCFiniteTimeAction *action = unTouchAction();
        if(action)
            runAction(
                CCSequence::create(
                    action,
                    CCCallFunc::create(this, callfunc_selector(CCSimpleButton::endTouching)),
                    NULL
                )
            );
    }

    void CCSimpleButton::callCallback()
    {
        endTouching();
        (target->*selector)(this);
    }

    CCFiniteTimeAction *CCSimpleButton::touchAction()
    {
        return CCScaleTo::create(0.1f, 0.9f);
    }

    CCFiniteTimeAction *CCSimpleButton::unTouchAction()
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
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, priority, true);
    }

    void CCSimpleButton::onExit()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        CCSprite::onExit();
    }

    void CCSimpleButton::setTappedEvent(CCObject *target, SEL_MenuHandler callback)
    {
        this->target = target;
        selector = callback;
    }

    void CCSimpleButton::setTouchPriority(const int priority)
    {
        this->priority = priority;
        if(CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this))
            CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(priority, this);
    }
}
