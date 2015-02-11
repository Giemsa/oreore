#include "ClippingNode.h"

namespace oreore
{
    namespace Tutorial
    {
        using namespace cocos2d;

        /* ClippingNode */
        const Rect *ClippingNode::getTouchRect() const
        {
            if(touchInfo.is<Rect>())
            {
                return &touchInfo.get<Rect>();
            }

            return nullptr;
        }

        const ClippingNode::CallbackFunc *ClippingNode::getTouchCallback() const
        {
            if(touchInfo.is<CallbackFunc>())
            {
                return &touchInfo.get<CallbackFunc>();
            }

            return nullptr;
        }

        bool ClippingNode::checkTouch(const Point &pos) const
        {
            if(touchInfo.empty())
            {
                CCASSERT(false, "invalid path");
                return false;
            }

            if(touchInfo.is<Rect>())
            {
                return touchInfo.get<Rect>().containsPoint(pos);
            }

            return touchInfo.get<CallbackFunc>()(pos);
        }
    }
}
