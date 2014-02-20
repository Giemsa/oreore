#include "Overlay.h"

namespace oreore
{
    using namespace cocos2d;

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
        syncChild = true;

        setZOrder(200);
        setAnchorPoint(Point::ZERO);
        LayerColor::setOpacity(0);
        setPosition(Point::ZERO);

        return true;
    }

    void OverlayLayer::show(const bool anime)
    {
        stopAllActions();

        if(anime)
        {
            LayerColor::setOpacity(0);
            runAction(
                Sequence::create(
                    FadeTo::create(displaySpeed, opacity),
                    CallFunc::create([this]() {
                        shown = true;
                        onShow();
                    }),
                    NULL
                )
            );
        }
        else
        {
            LayerColor::setOpacity(opacity);
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
                    NULL
                )
            );
        }
        else
        {
            LayerColor::setOpacity(0);
            shown = false;
            onClose();
        }
    }

    void OverlayLayer::setOpacity(GLubyte opacity)
    {
        if(syncChild)
        {
            setCascadeOpacityEnabled(true);
            const float r = std::min(static_cast<float>(opacity) / this->opacity, 1.0f);
            for(Node *child : getChildren())
                child->updateDisplayedOpacity(static_cast<GLubyte>(child->getOpacity() * r));
            setCascadeOpacityEnabled(false);
        }
        LayerColor::setOpacity(opacity);
    }

    void OverlayLayer::setChildrenVisible(const bool visible)
    {
        for(Node *child : getChildren())
            static_cast<Node *>(child)->setVisible(visible);
    }

    void OverlayLayer::setSyncOpecityChild(const bool sync)
    {
        syncChild = sync;
    }
}
