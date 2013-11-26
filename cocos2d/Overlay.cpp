#include "Overlay.h"

namespace cocos2d
{
    /* OverlayLayer */
    OverlayLayer::OverlayLayer() : opacity(0x90), displaySpeed(0.4f), shown(false)
    {
    
    }
    
    OverlayLayer::~OverlayLayer()
    {
    
    }

    OverlayLayer *OverlayLayer::create(const cocos2d::Color4B &color, const GLubyte opacity, const float speed)
    {
        OverlayLayer *r = new OverlayLayer();
        if(r && r->init(color, opacity, speed))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }
    
    bool OverlayLayer::init(const cocos2d::Color4B &color, const GLubyte opacity, const float speed)
    {
        if(!LayerColor::initWithColor(color))
            return false;
        
        this->opacity = opacity;
        displaySpeed = speed;
        
        setZOrder(200);
        setAnchorPoint(Point::ZERO);
        setOpacity(0);
        setPosition(Point::ZERO);
        
        return true;  
    }

    void OverlayLayer::show(const bool anime)
    {
        stopAllActions();
    
        if(anime)
        {
            setOpacity(0);
            runAction(
                Sequence::create(
                    FadeTo::create(displaySpeed, opacity),
                    CallFunc::create([this]() {
                        shown = true;
                        onShow();
                    }),
                    null
                )
            );
        }
        else
        {
            setOpacity(opacity);
            shown = true;
            onShow();
        }
    }
    
    void OverlayLayer::close(const bool anime)
    {
        if(!shown)
            return;

        if(anime)
        {
            runAction(
                Sequence::create(
                    FadeTo::create(displaySpeed, 0x00),
                    CallFunc::create([this]() {
                        shown = false;
                        onClose();
                    }),
                    null
                )
            );
        }
        else
        {
            shown = false;
            onClose();
        }
    }

    void OverlayLayer::setChildrenVisible(const bool visible)
    {
        Object *obj;
        CCARRAY_FOREACH(getChildren(), obj)
            static_cast<Node *>(obj)->setVisible(visible);
    }
}