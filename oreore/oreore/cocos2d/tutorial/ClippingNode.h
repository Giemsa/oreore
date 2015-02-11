#ifndef __OREORE_TUTORIAL_CLIPPINGNODE_H__
#define __OREORE_TUTORIAL_CLIPPINGNODE_H__

#include "../../utils/variant.hpp"

namespace oreore
{
    namespace Tutorial
    {
        class ClippingNode
        {
        private:
            bool touchEnabled;
        public:
            ClippingNode()
            : touchEnabled(true)
            { }

            virtual ~ClippingNode() { }
            /**
             * @brief くり抜き部分のタッチ入力を裏側のノードへ
             * 伝搬させるかの設定
             */
            void setTouchEnabled(const bool enable) { touchEnabled = enable; }
            bool isTouchEnabled() const { return touchEnabled; }
        };
    }
}

#endif
