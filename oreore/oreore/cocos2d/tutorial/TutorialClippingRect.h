#ifndef __OREORE_COCOS2DX_TUTORIALCLIPPINGRECT_H__
#define __OREORE_COCOS2DX_TUTORIALCLIPPINGRECT_H__

#include "cocos2d.h"

namespace oreore
{
    class TutorialClippingRect : public cocos2d::LayerColor
    {
    private:
        bool touchEnabled;

        bool init() override;
        bool init(const cocos2d::Rect &rect);
    public:
        CREATE_FUNC(TutorialClippingRect);
        static TutorialClippingRect *create(const cocos2d::Rect &rect);

        /**
         * @brief くり抜き部分のタッチ入力を裏側のノードへ
         * 伝搬させるかの設定
         */
        void setTouchEnabled(const bool enable) { touchEnabled = enable; }
        bool isTouchEnabled() const { return touchEnabled; }

        virtual bool hit(const cocos2d::Point &point);
    };
}

#endif
