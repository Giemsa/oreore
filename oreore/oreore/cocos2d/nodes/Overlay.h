#ifndef __OREORE_COCOS2D_OVERLAY__
#define __OREORE_COCOS2D_OVERLAY__

#include "cocos2d.h"

namespace oreore
{
    class OverlayLayer : public cocos2d::LayerColor
    {
    private:
        GLubyte opacity;
        float displaySpeed;
        bool shown, syncChild, animating;
        cocos2d::ccMenuCallback callbackOnShow, callbackOnClose;

        void onCompleteShow();
        void onCompleteClose();
    protected:
        OverlayLayer();
        virtual ~OverlayLayer();

        bool init() override { return init(cocos2d::Color4B(), 0x90, 0.4f); }
        bool init(const cocos2d::Color4B &color, const GLubyte opacity, const float speed);

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

        inline bool isShown() const { return shown; }

        virtual void show(const bool anime = true);
        virtual void close(const bool anime = true);

        void setOpacity(GLubyte opacity) override;
        void setSyncOpecityChild(const bool sync);

        void setOnShowCallback(const cocos2d::ccMenuCallback &callback);
        void setOnCloseCallback(const cocos2d::ccMenuCallback &callback);
        inline bool isAnimating() const { return animating; }
    };
}

#endif

