#include "ToggleButton.h"

namespace oreore
{
    using namespace cocos2d;

    /* CCToggleButton */
    bool CCToggleButton::forceSingleTouch = false;
    bool CCToggleButton::singleTouched = false;

    CCToggleButton *CCToggleButton::create(const std::string &offBtn, const std::string &onBtn)
    {
        CCToggleButton *r = new CCToggleButton();
        if(r && r->init(offBtn, onBtn))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    CCToggleButton *CCToggleButton::createWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn)
    {
        CCToggleButton *r = new CCToggleButton();
        if(r && r->initWithSpriteFrameName(offBtn, onBtn))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    bool CCToggleButton::init()
    {
        return init("", "");
    }

    bool CCToggleButton::init(const std::string &offBtn, const std::string &onBtn)
    {
        if(!CCSprite::init())
            return false;

        _init();
        if(!offBtn.empty())
        {
            CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(offBtn.c_str());
            offFrm = CCSpriteFrame::createWithTexture(tex, CCRect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(offFrm, offBtn.c_str());
        }
        
        if(!onBtn.empty())
        {
            CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(onBtn.c_str());
            onFrm = CCSpriteFrame::createWithTexture(tex, CCRect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(onFrm, onBtn.c_str());
        }

        fixSize();
        return true;
    }

    bool CCToggleButton::initWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn)
    {
        if(!CCSprite::init())
            return false;

        _init();
        if(!offBtn.empty())
            offFrm = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(offBtn.c_str());
        
        if(!onBtn.empty())
            onFrm = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(onBtn.c_str());

        fixSize();
        return true;
    }

    void CCToggleButton::_init()
    {
        onFrm = null;
        offFrm = null;
        target = null;
        selector = null;
        touchEnabled = true;
        allowContinuousHit = false;
    }

    void CCToggleButton::fixSize()
    {
        if(!offFrm && !onFrm)
            return;

        CCSize onRect, offRect;
        bool set = false;
        if(offFrm)
        {
            offRect = offFrm->getRect().size;
            setDisplayFrame(offFrm);
            set = true;
        }

        if(onFrm)
        {
            onRect = onFrm->getRect().size;
            if(!set)
                setDisplayFrame(onFrm);
        }

        setContentSize(CCSize(std::max(offRect.width, onRect.width), std::max(offRect.height, onRect.height)));
    }

    CCFiniteTimeAction *CCToggleButton::touchAction()
    {
        return CCScaleTo::create(0.1f, 0.9f);
    }

    CCFiniteTimeAction *CCToggleButton::unTouchAction()
    {
        return CCScaleTo::create(0.1f, 1.0f);
    }

    void CCToggleButton::toggleAndAction()
    {
        endTouching();
        if(target && selector)
            (target->*selector)(this);
        
        toggle();
    }

    void CCToggleButton::endTouching()
    {
        touched = false;
        singleTouched = false;
        CCLOG("singleTouched = false");
    }

    bool CCToggleButton::ccTouchBegan(CCTouch *touch, CCEvent *event)
    {
        if(forceSingleTouch && singleTouched)
            return false;

        if(!touchEnabled || !isVisible())
            return false;

        if(!allowContinuousHit && touched)
            return false;

        const CCPoint &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(boundingBox().containsPoint(p))
        {
            touched = true;
            singleTouched = true;
            CCFiniteTimeAction *action = touchAction();
            if(action)
                runAction(action);
            return true;
        }

        return false;
    }

    void CCToggleButton::ccTouchMoved(CCTouch *touch, CCEvent *event)
    {

    }

    void CCToggleButton::ccTouchEnded(CCTouch *touch, CCEvent *event)
    {
        const CCPoint &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(boundingBox().containsPoint(p))
        {
            CCFiniteTimeAction *a = unTouchAction();
            if(!a)
                toggleAndAction();
            else
                runAction(
                    CCSpawn::create(
                        a,
                        CCSequence::create(
                            CCDelayTime::create(a->getDuration() / 2.0),
                            CCCallFunc::create(this, callfunc_selector(CCToggleButton::toggleAndAction)),
                            NULL
                        ),
                        NULL
                    )
                );
            return;
        }

        CCFiniteTimeAction *action = unTouchAction();
        if(action)
            runAction(
                CCSequence::create(
                    action,
                    CCCallFunc::create(this, callfunc_selector(CCToggleButton::endTouching)),
                    NULL
                )
            );
    }

    void CCToggleButton::setTouchEnabled(const bool enable)
    {
        touchEnabled = enable;
    }

    bool CCToggleButton::isTouchEnabled() const
    {
        return touchEnabled;
    }

    void CCToggleButton::toggleOn()
    {
        if(!onFrm || !offFrm || toggled)
            return;

        setDisplayFrame(onFrm);
        toggled = true;
    }

    void CCToggleButton::toggleOff()
    {
        if(!onFrm || !offFrm || !toggled)
            return;

        setDisplayFrame(offFrm);
        toggled = false;
    }

    void CCToggleButton::toggle()
    {
        if(toggled)
            toggleOff();
        else
            toggleOn();
    }

    void CCToggleButton::toggle(const bool v)
    {
        if(v)
            toggleOn();
        else
            toggleOff();
    }

    void CCToggleButton::setTappedEvent(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler callback)
    {
        this->target = target;
        selector = callback;
    }

    void CCToggleButton::onEnter()
    {
        CCNode::onEnter();
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
    }

    void CCToggleButton::onExit()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        CCNode::onExit();
    }
}