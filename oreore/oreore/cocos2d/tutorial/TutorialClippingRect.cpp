#include "TutorialClippingRect.h"
#include "../utils/Utils.h"

namespace oreore
{
    using namespace cocos2d;

    /* TutorialClippingRect */
    TutorialClippingRect *TutorialClippingRect::create(const Rect &rect)
    {
        TutorialClippingRect *r = new TutorialClippingRect();
        if(r && r->init(rect))
        {
            r->autorelease();
            return r;
        }

        return nullptr;
    }

    bool TutorialClippingRect::init()
    {
        return init(makeRect(Point::ZERO, Director::getInstance()->getWinSize()));
    }

    bool TutorialClippingRect::init(const Rect &rect)
    {
        if(!LayerColor::initWithColor(Color4B::WHITE, rect.size.width, rect.size.height))
        {
            return false;
        }

        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Point::ANCHOR_MIDDLE);
        setPosition(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);

        touchEnabled = true;
        return true;
    }

    bool TutorialClippingRect::hit(const Point &point)
    {
        return getBoundingBox().containsPoint(point);
    }
}
