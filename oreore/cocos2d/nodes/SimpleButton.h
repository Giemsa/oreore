#ifndef __OREORE_COCOS2D_SIMPLEBUTTON_H__
#define __OREORE_COCOS2D_SIMPLEBUTTON_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class CCSimpleButton :
        public cocos2d::CCSprite,
        public cocos2d::CCTargetedTouchDelegate
    {
    private:
        cocos2d::SEL_MenuHandler selector;
        cocos2d::CCObject *target;
        cocos2d::CCPoint bpos;
        bool touchEnabled;

        virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
        virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
        virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

        void _init(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    public:
        static CCSimpleButton* create(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        static CCSimpleButton* create(const char *pszFileName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        static CCSimpleButton* create(const char *pszFileName, const cocos2d::CCRect& rect, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        static CCSimpleButton* createWithTexture(cocos2d::CCTexture2D *pTexture, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        static CCSimpleButton* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        static CCSimpleButton* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        static CCSimpleButton* createWithSpriteFrameName(const char *pszSpriteFrameName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

        inline static CCSimpleButton* create() { return create(null, null); }
        inline static CCSimpleButton* create(const char *pszFileName) { return create(pszFileName, null, null); }
        inline static CCSimpleButton* create(const char *pszFileName, const cocos2d::CCRect& rect) { return create(pszFileName, rect, null, null); }
        inline static CCSimpleButton* createWithTexture(cocos2d::CCTexture2D *pTexture) { return createWithTexture(pTexture, null, null); }
        inline static CCSimpleButton* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect) { return createWithTexture(pTexture, rect, null, null); }
        inline static CCSimpleButton* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame) { return createWithSpriteFrame(pSpriteFrame, null, null); }
        inline static CCSimpleButton* createWithSpriteFrameName(const char *pszSpriteFrameName) { return createWithSpriteFrameName(pszSpriteFrameName, null, null); }

        virtual bool init(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, bool rotated, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithFile(const char *pszFilename, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
        virtual bool initWithFile(const char *pszFilename, const cocos2d::CCRect& rect, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

        virtual cocos2d::CCAction *touchAction();
        virtual cocos2d::CCAction *unTouchAction();

        virtual void setTouchEnabled(const bool enable);
        virtual bool getTouchEnabled() const;

        virtual void onEnter(); // override
        virtual void onExit(); // override
    };
}

#endif
