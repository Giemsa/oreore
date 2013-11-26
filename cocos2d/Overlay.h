#ifndef __COCOS2D_EXT_OVERLAY__
#define __COCOS2D_EXT_OVERLAY__

#include "cocos2d.h"
#include "../null.h"

namespace cocos2d
{
    // オーバーレイレイヤ
    class OverlayLayer : public cocos2d::LayerColor
    {
    private:
        GLubyte opacity;
        float displaySpeed;
        bool shown;
    protected:
        virtual void onShow() { }
        virtual void onClose() { }

        void setChildrenVisible(const bool visible);
    public:
        inline static OverlayLayer *create()
        {
            return create(cocos2d::Color4B(), 0x90, 0.4f);
        }

        inline static OverlayLayer *create(const cocos2d::Color4B &color)
        {
            return create(color, 0x90, 0.4f);
        }

        inline static OverlayLayer *create(const GLubyte opacity, const float speed)
        {
            return create(cocos2d::Color4B(), opacity, speed);
        }

        static OverlayLayer *create(const cocos2d::Color4B &color, const GLubyte opacity, const float speed);

        OverlayLayer();
        virtual ~OverlayLayer();

        bool init() override { return init(cocos2d::Color4B(), 0x90, 0.4f); }
        bool init(const cocos2d::Color4B &color, const GLubyte opacity, const float speed);

        inline bool isShown() const { return shown; }

        virtual void show(const bool anime = true);
        virtual void close(const bool anime = true);
    };
}

#endif /* defined(__REFLEX__TextAnimation__) */
