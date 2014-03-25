#ifndef __OREORE_COCOS2D_SIMPLEBUTTON_H__
#define __OREORE_COCOS2D_SIMPLEBUTTON_H__

#include "cocos2d.h"
#include "../../null.h"
#include <limits>

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

        virtual cocos2d::CCFiniteTimeAction *touchAction();
        virtual cocos2d::CCFiniteTimeAction *unTouchAction();

        void callCallback();

        virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
        virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
        virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

        void _init();
    public:
        CREATE_FUNC(CCSimpleButton);
        virtual ~CCSimpleButton() { }
    
        static CCSimpleButton* create(const char *pszFileName);
        static CCSimpleButton* createWithTexture(cocos2d::CCTexture2D *pTexture);
        static CCSimpleButton* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
        static CCSimpleButton* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame);
        static CCSimpleButton* createWithSpriteFrameName(const char *pszSpriteFrameName);

        virtual bool init(); // override
        virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture);
        virtual bool initWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
        virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame);
        virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName);
        virtual bool initWithFile(const char *pszFilename);
        virtual bool initWithFile(const char *pszFilename, const cocos2d::CCRect& rect);

        virtual void setTouchEnabled(const bool enable);
        virtual bool getTouchEnabled() const;

        virtual void onEnter(); // override
        virtual void onExit(); // override

        void setTappedEvent(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler callback);
    };

    template<typename T>
    class CCExtendableSimpleButton : public CCSimpleButton
    {
    private:
    public:
        CREATE_FUNC(T);

        virtual ~CCExtendableSimpleButton() { }

        T* createWithTexture(cocos2d::CCTexture2D *pTexture)
        {
            T *pobSprite = new T();
            if(pobSprite && pobSprite->initWithTexture(pTexture))
            {
                pobSprite->autorelease();
                return pobSprite;
            }
            CC_SAFE_DELETE(pobSprite);
            return NULL;
        }

        static T* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect)
        {
            T *pobSprite = new T();
            if(pobSprite && pobSprite->initWithTexture(pTexture, rect))
            {
                pobSprite->autorelease();
                return pobSprite;
            }
            CC_SAFE_DELETE(pobSprite);
            return NULL;
        }

        static T* create(const char *pszFileName)
        {
            T *pobSprite = new T();
            if(pobSprite && pobSprite->initWithFile(pszFileName))
            {
                pobSprite->autorelease();
                return pobSprite;
            }
            CC_SAFE_DELETE(pobSprite);
            return NULL;
        }

        static T* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame)
        {
            T *pobSprite = new T();
            if(pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
            {
                pobSprite->autorelease();
                return pobSprite;
            }
            CC_SAFE_DELETE(pobSprite);
            return NULL;
        }

        static T* createWithSpriteFrameName(const char *pszSpriteFrameName)
        {
            cocos2d::CCSpriteFrame *pFrame = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);

        #if COCOS2D_DEBUG > 0
            char msg[256] = {0};
            sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
            CCAssert(pFrame != NULL, msg);
        #endif

            return createWithSpriteFrame(pFrame);
        }
    };
}

#endif
