#include "ClippingRect.h"
#include "../utils/Utils.h"
#include "TutorialBase.h"

namespace oreore
{
    namespace Tutorial
    {
        using namespace cocos2d;

        /* ClippingRect */
        ClippingRect *ClippingRect::create(const float width, const float height)
        {
            ClippingRect *r = new ClippingRect();
            if(r && r->init(width, height))
            {
                r->autorelease();
                return r;
            }

            return nullptr;
        }

        bool ClippingRect::init(const float width, const float height)
        {
            if(!LayerColor::initWithColor(Color4B::WHITE, width, height))
            {
                return false;
            }

            ignoreAnchorPointForPosition(false);
            setAnchorPoint(Point::ANCHOR_MIDDLE);
            setGlobalZOrder(detail::TutorialBaseBase::DefaultZOrder);
            setBlendFunc({ GL_DST_COLOR, GL_ONE });
            ClippingNode::setTouchEnabled(true);
            return true;
        }

        bool ClippingRect::checkTouch(const cocos2d::Point &pos) const
        {
            if(hasTouchHandle())
            {
                return ClippingNode::checkTouch(pos);
            }

            return getBoundingBox().containsPoint(pos);
        }
    }
}
