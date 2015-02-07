#ifndef __OREORE_TUTORIAL_CLIPPINGRECT_H__
#define __OREORE_TUTORIAL_CLIPPINGRECT_H__

#include "cocos2d.h"
#include "ClippingNode.h"

namespace oreore
{
    namespace Tutorial
    {
        class ClippingRect
            : public cocos2d::LayerColor
            , public ClippingNode
        {
        private:
            bool init(const float width, const float height);
        public:
            using ClippingNode::setTouchEnabled;
            using ClippingNode::isTouchEnabled;

            static ClippingRect *create(const float width, const float height);
        };
    }
}

#endif
