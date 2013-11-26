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
    
    bool OverlayLayer::init()
    {
        if(!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
            return false;
        
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
            setOpacity(0xFF);
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