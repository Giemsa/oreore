#ifndef __OREORE_COCOS2D_OVERLAY__
#define __OREORE_COCOS2D_OVERLAY__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class CCOverlayLayer : public cocos2d::CCLayerColor
    {
    private:
        GLubyte opacity;
        float displaySpeed;
        bool shown, syncChild;

        cocos2d::CCObject *targetOnShow, *targetOnClose;
        cocos2d::SEL_MenuHandler callbackOnShow, callbackOnClose;

        void onCompleteShow();
        void onCompleteClose();
    protected:
        virtual void onShow() { }
        virtual void onClose() { }

        void setChildrenVisible(const bool visible);
    public:
        inline static CCOverlayLayer *create()
        {
            return create(cocos2d::ccColor4B(), 0x90, 0.4f);
        }

        inline static CCOverlayLayer *create(const cocos2d::ccColor4B &color)
        {
            return create(color, 0x90, 0.4f);
        }

        inline static CCOverlayLayer *create(const GLubyte opacity, const float speed)
        {
            return create(cocos2d::ccColor4B(), opacity, speed);
        }

        static CCOverlayLayer *create(const cocos2d::ccColor4B &color, const GLubyte opacity, const float speed);

        CCOverlayLayer();
        virtual ~CCOverlayLayer();

        bool init() { return init(cocos2d::ccColor4B(), 0x90, 0.4f); } // override
        bool init(const cocos2d::ccColor4B &color, const GLubyte opacity, const float speed);

        inline bool isShown() const { return shown; }

        virtual void show(const bool anime = true);
        virtual void close(const bool anime = true);

        void setOpacity(GLubyte opacity); // override
        void setSyncOpecityChild(const bool sync);

        void setOnShowCallback(cocos2d::CCObject *target, const cocos2d::SEL_MenuHandler callback);
        void setOnCloseCallback(cocos2d::CCObject *target, const cocos2d::SEL_MenuHandler callback);
    };
}

#endif
