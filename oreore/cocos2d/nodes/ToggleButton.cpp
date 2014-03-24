#include "ToggleButton.h"

namespace oreore
{
    using namespace cocos2d;

    /* ToggleButton */
    ToggleButton *ToggleButton::create(const std::string &offBtn, const std::string &onBtn)
    {
        ToggleButton *r = new ToggleButton();
        if(r && r->init(offBtn, onBtn))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    ToggleButton *ToggleButton::createWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn)
    {
        ToggleButton *r = new ToggleButton();
        if(r && r->initWithSpriteFrameName(offBtn, onBtn))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    bool ToggleButton::init()
    {
        return init("", "");
    }

    bool ToggleButton::init(const std::string &offBtn, const std::string &onBtn)
    {
        if(!CCSprite::init())
            return false;

        _init();
        if(!offBtn.empty())
        {
            Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(offBtn.c_str());
            offFrm = SpriteFrame::createWithTexture(tex, Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(offFrm, offBtn.c_str());
        }
        
        if(!onBtn.empty())
        {
            Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(onBtn.c_str());
            onFrm = SpriteFrame::createWithTexture(tex, Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(onFrm, onBtn.c_str());
        }

        fixSize();
        return true;
    }

    bool ToggleButton::initWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn)
    {
        if(!Sprite::init())
            return false;

        _init();
        if(!offBtn.empty())
            offFrm = SpriteFrameCache::getInstance()->getSpriteFrameByName(offBtn.c_str());
        
        if(!onBtn.empty())
            onFrm = SpriteFrameCache::getInstance()->getSpriteFrameByName(onBtn.c_str());

        fixSize();
        return true;
    }

    void ToggleButton::_init()
    {
        onFrm = null;
        offFrm = null;
        callback = null;
        touchEnabled = true;
    }

    void ToggleButton::fixSize()
    {
        if(!offFrm && !onFrm)
            return;

        Size onRect, offRect;
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

        setContentSize(Size(std::max(offRect.width, onRect.width), std::max(offRect.height, onRect.height)));
    }

    FiniteTimeAction *ToggleButton::action()
    {
        return null;
    }

    void ToggleButton::toggleAndAction()
    {
        if(callback)
            callback(this);
        toggle();
    }

    bool ToggleButton::onTouchBegan(Touch *touch, Event *event)
    {
        if(!touchEnabled)
            return false;


        const Point &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(getBoundingBox().containsPoint(p))
            return true;

        return false;
    }


    void ToggleButton::onTouchEnded(Touch *touch, Event *event)
    {
        const Point &p = getParent()->convertToNodeSpace(touch->getLocation());
        if(getBoundingBox().containsPoint(p))
        {
            FiniteTimeAction *a = action();
            if(!a)
                toggleAndAction();
            else
            {
                runAction(
                    Spawn::create(
                        a,
                        Sequence::create(
                            DelayTime::create(a->getDuration() / 2.0),
                            CallFunc::create(CC_CALLBACK_0(ToggleButton::toggleAndAction, this)),
                            NULL
                        ),
                        NULL
                    )
                );
            }
        }
    }

    void ToggleButton::setTouchEnabled(const bool enable)
    {
        touchEnabled = enable;
    }

    bool ToggleButton::isTouchEnabled() const
    {
        return touchEnabled;
    }

    void ToggleButton::toggleOn()
    {
        if(!onFrm || !offFrm || toggled)
            return;

        setDisplayFrame(onFrm);
        toggled = true;
    }

    void ToggleButton::toggleOff()
    {
        if(!onFrm || !offFrm || !toggled)
            return;

        setDisplayFrame(offFrm);
        toggled = false;
    }

    void ToggleButton::toggle()
    {
        if(toggled)
            toggleOff();
        else
            toggleOn();
    }

    void ToggleButton::setTappedEvent(const ccMenuCallback &callback)
    {
        this->callback = callback;
    }

    void ToggleButton::onEnter()
    {
        Node::onEnter();
        listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ToggleButton::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(ToggleButton::onTouchEnded, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }

    void ToggleButton::onExit()
    {
        getEventDispatcher()->removeEventListener(listener);
        Node::onExit();
    }
}
