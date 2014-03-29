#include "Overlay.h"

namespace oreore
{
    using namespace cocos2d;

    /* CCOverlayLayer */
    CCOverlayLayer::CCOverlayLayer() :
        opacity(0x90), displaySpeed(0.4f), shown(false), syncChild(true), animating(false),
        targetOnShow(null), targetOnClose(null),
        callbackOnShow(null), callbackOnClose(null)
    {
    }

    CCOverlayLayer::~CCOverlayLayer()
    {

    }

    CCOverlayLayer *CCOverlayLayer::create(const cocos2d::ccColor4B &color, const GLubyte opacity, const float speed)
    {
        CCOverlayLayer *r = new CCOverlayLayer();
        if(r && r->init(color, opacity, speed))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    bool CCOverlayLayer::init(const cocos2d::ccColor4B &color, const GLubyte opacity, const float speed)
    {
        if(!CCLayerColor::initWithColor(color))
            return false;

        this->opacity = opacity;
        displaySpeed = speed;
        syncChild = true;

        setZOrder(200);
        setAnchorPoint(CCPointZero);
        CCLayerColor::setOpacity(0);
        setPosition(CCPointZero);

        return true;
    }

    void CCOverlayLayer::onCompleteShow()
    {
        shown = true;
        animating = false;
        if(targetOnShow && callbackOnShow)
            (targetOnShow->*callbackOnShow)(this);
        onShow();
    }

    void CCOverlayLayer::show(const bool anime)
    {
        stopAllActions();

        if(anime)
        {
            animating = true;
            CCLayerColor::setOpacity(0);
            runAction(
                CCSequence::create(
                    CCFadeTo::create(displaySpeed, opacity),
                    CCCallFunc::create(this, callfunc_selector(CCOverlayLayer::onCompleteShow)),
                    NULL
                )
            );
        }
        else
        {
            CCLayerColor::setOpacity(opacity);
            shown = true;
            if(targetOnShow && callbackOnShow)
                (targetOnShow->*callbackOnShow)(this);
            onShow();
        }
    }

    void CCOverlayLayer::onCompleteClose()
    {
        shown = false;
        animating = false;
        if(targetOnClose && callbackOnClose)
            (targetOnClose->*callbackOnClose)(this);
        onClose();
    }

    void CCOverlayLayer::close(const bool anime)
    {
        if(!shown)
            return;

        if(anime)
        {
            animating = true;
            runAction(
                CCSequence::create(
                    CCFadeTo::create(displaySpeed, 0x00),
                    CCCallFunc::create(this, callfunc_selector(CCOverlayLayer::onCompleteClose)),
                    NULL
                )
            );
        }
        else
        {
            CCLayerColor::setOpacity(0);
            shown = false;
            onClose();
        }
    }

    void CCOverlayLayer::setOpacity(GLubyte opacity)
    {
        if(syncChild)
        {
            setCascadeOpacityEnabled(true);
            CCObject* child;
            const float r = std::min(static_cast<float>(opacity) / this->opacity, 1.0f);
            CCARRAY_FOREACH(getChildren(), child)
            {
                CCRGBAProtocol *p = dynamic_cast<CCRGBAProtocol*>(child);
                if(p)
                    p->updateDisplayedOpacity(static_cast<GLubyte>(p->getOpacity() * r));
            }
            setCascadeOpacityEnabled(false);
        }
        CCLayerColor::setOpacity(opacity);
    }

    void CCOverlayLayer::setChildrenVisible(const bool visible)
    {
        CCObject *obj;
        CCARRAY_FOREACH(getChildren(), obj)
            static_cast<CCNode *>(obj)->setVisible(visible);
    }

    void CCOverlayLayer::setSyncOpecityChild(const bool sync)
    {
        syncChild = sync;
    }

    void CCOverlayLayer::setOnShowCallback(cocos2d::CCObject *target, const cocos2d::SEL_MenuHandler callback)
    {
        targetOnShow = target;
        callbackOnShow = callback;
    }

    void CCOverlayLayer::setOnCloseCallback(cocos2d::CCObject *target, const cocos2d::SEL_MenuHandler callback)
    {
        targetOnClose = target;
        callbackOnClose = callback;
    }
}
