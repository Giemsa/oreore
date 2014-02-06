#include "Overlay.h"

namespace oreore
{
    using namespace cocos2d;

    /* CCOverlayLayer */
    CCOverlayLayer::CCOverlayLayer() : opacity(0x90), displaySpeed(0.4f), shown(false)
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

        setZOrder(200);
        setAnchorPoint(CCPointZero);
        CCLayerColor::setOpacity(0);
        setPosition(CCPointZero);

        return true;
    }

    void CCOverlayLayer::onCompleteShow()
    {
        shown = true;
        onShow();
    }

    void CCOverlayLayer::show(const bool anime)
    {
        stopAllActions();

        if(anime)
        {
            CCLayerColor::setOpacity(0);
            runAction(
                CCSequence::create(
                    CCFadeTo::create(displaySpeed, opacity),
                    CCCallFunc::create(this, callfunc_selector(CCOverlayLayer::onCompleteShow)),
                    null
                )
            );
        }
        else
        {
            CCLayerColor::setOpacity(opacity);
            shown = true;
            onShow();
        }
    }

    void CCOverlayLayer::onCompleteClose()
    {
        shown = false;
        onClose();
    }

    void CCOverlayLayer::close(const bool anime)
    {
        if(!shown)
            return;

        if(anime)
        {
            runAction(
                CCSequence::create(
                    CCFadeTo::create(displaySpeed, 0x00),
                    CCCallFunc::create(this, callfunc_selector(CCOverlayLayer::onCompleteClose)),
                    null
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
        CCLayerColor::setOpacity(opacity);
    }

    void CCOverlayLayer::setChildrenVisible(const bool visible)
    {
        CCObject *obj;
        CCARRAY_FOREACH(getChildren(), obj)
            static_cast<CCNode *>(obj)->setVisible(visible);
    }
}
