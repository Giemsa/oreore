#ifndef __OREORE_COCOS2D_SIMPLEBUTTON_H__
#define __OREORE_COCOS2D_SIMPLEBUTTON_H__

#include "cocos2d.h"
#include "../../null.h"
#include <limits>

namespace oreore
{
    class SimpleButton : public cocos2d::Sprite
    {
    private:
        cocos2d::ccMenuCallback selector;
        cocos2d::Point bpos;
        cocos2d::EventListenerTouchOneByOne *listener;
        bool touchEnabled, touched, allowContinuousHit;
        int priority;
        static bool forceSingleTouch, singleTouched;

        virtual cocos2d::FiniteTimeAction *touchAction();
        virtual cocos2d::FiniteTimeAction *unTouchAction();

        void callCallback();
        void endTouching();

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

        void _init();
    protected:
        virtual ~SimpleButton() { }

        virtual bool init();
        virtual bool initWithTexture(cocos2d::Texture2D *pTexture);
        virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect);
        virtual bool initWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect, bool rotated);
        virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame);

        virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName);
        virtual bool initWithFile(const char *pszFilename);
        virtual bool initWithFile(const char *pszFilename, const cocos2d::Rect& rect);
    public:
        CREATE_FUNC(SimpleButton);
        static SimpleButton* create(const char *pszFileName);
        static SimpleButton* create(const char *pszFileName, const cocos2d::Rect& rect);
        static SimpleButton* createWithTexture(cocos2d::Texture2D *pTexture);
        static SimpleButton* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect);
        static SimpleButton* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame);
        static SimpleButton* createWithSpriteFrameName(const char *pszSpriteFrameName);

        virtual void setTouchEnabled(const bool enable);
        virtual bool getTouchEnabled() const;

        virtual void onEnter() override;
        virtual void onExit() override;

        void setTappedEvent(const cocos2d::ccMenuCallback &callback);
        void setTouchPriority(const int priority);
        inline void setAllowContinuousHit(const bool allow) { allowContinuousHit = allow; }
        inline bool isAllowContinuousHit() const { return allowContinuousHit; }
        inline static void setForceSingleTouchMode(const bool enable) { forceSingleTouch = enable; }
        inline static bool isForceSingeTouchMode() { return forceSingleTouch; }
    };

    template<typename T>
    class ExtendableSimpleButton : public SimpleButton
    {
    private:
    protected:
        virtual ~ExtendableSimpleButton() { }
    public:
        CREATE_FUNC(T);

        T* createWithTexture(cocos2d::Texture2D *pTexture)
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

        static T* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect)
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

        static T* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame)
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
            cocos2d::SpriteFrame *pFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);

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
