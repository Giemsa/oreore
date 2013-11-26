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
        CREATE_FUNC(OverlayLayer);
        static OverlayLayer *create(const cocos2d::Color4B &color);
        static OverlayLayer *create(const cocos2d::Color4B &color, const GLubyte opacity, const float speed);
        static OverlayLayer *create(const GLubyte opacity, const float speed);

        OverlayLayer();
        virtual ~OverlayLayer();

        bool init() override;
        bool init(const cocos2d::Color4B &color);
        bool init(const cocos2d::Color4B &color, const GLubyte opacity, const float speed);
        bool init(const GLubyte opacity, const float speed);

        inline bool isShown() const { return shown; }

        virtual void show(const bool anime = true);
        virtual void close(const bool anime = true);
    };
}

#endif /* defined(__REFLEX__TextAnimation__) */
