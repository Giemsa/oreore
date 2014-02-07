#ifndef __OREORE_COCOS2D_SIMPLEBUTTON_H__
#define __OREORE_COCOS2D_SIMPLEBUTTON_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class SimpleButton : public cocos2d::Sprite
    {
    private:
        cocos2d::ccMenuCallback selector;
        cocos2d::Point bpos;

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

        void _init(const cocos2d::ccMenuCallback &selector);
    public:
        static SimpleButton* create(const cocos2d::ccMenuCallback &selector);
        static SimpleButton* create(const char *pszFileName, const cocos2d::ccMenuCallback &selector);
        static SimpleButton* create(const char *pszFileName, const cocos2d::Rect& rect, const cocos2d::ccMenuCallback &selector);
        static SimpleButton* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::ccMenuCallback &selector);
        static SimpleButton* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, const cocos2d::ccMenuCallback &selector);
        static SimpleButton* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame, const cocos2d::ccMenuCallback &selector);
        static SimpleButton* createWithSpriteFrameName(const char *pszSpriteFrameName, const cocos2d::ccMenuCallback &selector);

        inline static SimpleButton* create() { return create(null); }
        inline static SimpleButton* create(const char *pszFileName) { return create(pszFileName, null); }
        inline static SimpleButton* create(const char *pszFileName, const cocos2d::Rect& rect) { return create(pszFileName, rect, null); }
        inline static SimpleButton* createWithTexture(cocos2d::Texture2D *pTexture) { return createWithTexture(pTexture, null); }
        inline static SimpleButton* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect &rect) { return createWithTexture(pTexture, rect, null); }
        inline static SimpleButton* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame) { return createWithSpriteFrame(pSpriteFrame, null); }
        inline static SimpleButton* createWithSpriteFrameName(const char *pszSpriteFrameName) { return createWithSpriteFrameName(pszSpriteFrameName, null); }

        virtual bool init(const cocos2d::ccMenuCallback &selector);
        virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::ccMenuCallback &selector);
        virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, const cocos2d::ccMenuCallback &selector);
        virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated, const cocos2d::ccMenuCallback &selector);
        virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame, const cocos2d::ccMenuCallback &selector);
        virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName, const cocos2d::ccMenuCallback &selector);
        virtual bool initWithFile(const char *pszFilename, const cocos2d::ccMenuCallback &selector);
        virtual bool initWithFile(const char *pszFilename, const cocos2d::Rect& rect, const cocos2d::ccMenuCallback &selector);

        virtual cocos2d::Action *touchAction();
        virtual cocos2d::Action *unTouchAction();
    };
}

#endif
