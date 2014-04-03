#include "SimpleButton.h"

namespace oreore
{
    using namespace cocos2d;

    /* SimpleButton */
    bool SimpleButton::forceSingleTouch = false;
    bool SimpleButton::singleTouched = false;

    SimpleButton* SimpleButton::createWithTexture(Texture2D *pTexture)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithTexture(pTexture))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::createWithTexture(Texture2D *pTexture, const Rect& rect)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::create(const char *pszFileName)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithFile(pszFileName))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::create(const char *pszFileName, const Rect& rect)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::createWithSpriteFrame(SpriteFrame *pSpriteFrame)
    {
        SimpleButton *pobSprite = new SimpleButton();
        if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }

    SimpleButton* SimpleButton::createWithSpriteFrameName(const char *pszSpriteFrameName)
    {
        SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);

    #if COCOS2D_DEBUG > 0
        char msg[256] = {0};
        sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
        CCAssert(pFrame != NULL, msg);
    #endif

        return createWithSpriteFrame(pFrame);
    }

    void SimpleButton::_init()
    {
        selector = null;
        touchEnabled = true;
        priority = 0;
        touched = false;
        allowContinuousHit = false;
    }

    bool SimpleButton::init()
    {
        _init();
        return Sprite::init();
    }

    bool SimpleButton::initWithTexture(Texture2D *pTexture)
    {
        _init();
        return Sprite::initWithTexture(pTexture);
    }

    bool SimpleButton::initWithTexture(Texture2D *pTexture, const Rect& rect)
    {
        _init();
        return Sprite::initWithTexture(pTexture, rect);
    }

    bool SimpleButton::initWithTexture(Texture2D *pTexture, const Rect& rect, bool rotated)
    {
        _init();
        return Sprite::initWithTexture(pTexture, rect, rotated);
    }

    bool SimpleButton::initWithSpriteFrame(SpriteFrame *pSpriteFrame)
    {
        _init();
        return Sprite::initWithSpriteFrame(pSpriteFrame);
    }

    bool SimpleButton::initWithSpriteFrameName(const char *pszSpriteFrameName)
    {
        _init();
        return Sprite::initWithSpriteFrameName(pszSpriteFrameName);
    }

    bool SimpleButton::initWithFile(const char *pszFilename)
    {
        _init();
        return Sprite::initWithFile(pszFilename);
    }

    bool SimpleButton::initWithFile(const char *pszFilename, const Rect& rect)
    {
        _init();
        return Sprite::initWithFile(pszFilename, rect);
    }

    void SimpleButton::endTouching()
    {
        touched = false;
        singleTouched = false;
        CCLOG("singleTouched = false");
    }

    bool SimpleButton::onTouchBegan(Touch *touch, Event *event)
    {
        if(forceSingleTouch && singleTouched)
            return false;

        if(!touchEnabled || !isVisible())
            return false;

        if(!allowContinuousHit && touched)
            return false;

        const Point &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(getBoundingBox().containsPoint(p))
        {
            touched = true;
            singleTouched = true;
            CCLOG("singleTouched = true");
            bpos = p;
            FiniteTimeAction *action = touchAction();
            if(action)
                runAction(action);
            return true;
        }
        return false;
    }

    void SimpleButton::onTouchEnded(Touch *touch, Event *event)
    {
        const Point &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(getBoundingBox().containsPoint(p))
        {
            if(selector)
            {
                FiniteTimeAction *action = unTouchAction();
                if(action)
                    runAction(
                        CCSpawn::create(
                            action,
                            CCSequence::create(
                                CCDelayTime::create(action->getDuration() / 2.0),
                                CallFunc::create(CC_CALLBACK_0(SimpleButton::callCallback, this)),
                                NULL
                            ),
                            NULL
                        )
                    );
                return;
            }
        }

        FiniteTimeAction *action = unTouchAction();
        if(action)
            runAction(
                CCSequence::create(
                    action,
                    CallFunc::create(CC_CALLBACK_0(SimpleButton::endTouching, this)),
                    NULL
                )
            );
    }

    void SimpleButton::callCallback()
    {
        endTouching();
        selector(this);
    }


    void SimpleButton::setTouchEnabled(const bool enable)
    {
        touchEnabled = enable;
    }

    bool SimpleButton::getTouchEnabled() const
    {
        return touchEnabled;
    }

    FiniteTimeAction *SimpleButton::touchAction()
    {
        return ScaleTo::create(0.1f, 0.9f);
    }

    FiniteTimeAction *SimpleButton::unTouchAction()
    {
        return ScaleTo::create(0.1f, 1.0f);
    }

    void SimpleButton::onEnter()
    {
        Sprite::onEnter();
        listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(SimpleButton::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(SimpleButton::onTouchEnded, this);
        if(priority == 0)
            getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        else
            getEventDispatcher()->addEventListenerWithFixedPriority(listener, priority);
    }

    void SimpleButton::onExit()
    {
        getEventDispatcher()->removeEventListener(listener);
        Sprite::onExit();
    }

    void SimpleButton::setTappedEvent(const ccMenuCallback &callback)
    {
        selector = callback;
    }

    void SimpleButton::setTouchPriority(const int priority)
    {
        this->priority = priority;
        getEventDispatcher()->setPriority(listener, priority);
    }
}
