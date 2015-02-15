#ifndef __OREORE_COCOS2DX_TUTORIALBASE_H__
#define __OREORE_COCOS2DX_TUTORIALBASE_H__

#include "cocos2d.h"
#include "TutorialPhase.h"
#include "ClippingSprite.h"
#include "ClippingScale9Sprite.h"
#include "ClippingRect.h"

namespace oreore
{
    namespace Tutorial
    {
        // forward
        template<typename T, typename D>
        class TutorialManager; // in TutorialManager.h

        namespace detail
        {
            class TutorialBaseBase : public cocos2d::Node
            {
                template<typename T, typename D>
                friend class oreore::Tutorial::TutorialManager;
            public:
                static constexpr float DefaultZOrder = 10240.0f;
            private:
                cocos2d::LayerColor *maskLayer;
                cocos2d::EventListenerTouchOneByOne *listener;
                bool completed;
                bool locked;
                bool touchEnabled;
                float fadeSpeed;
                size_t seq_id;

                bool init() override;

                bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
                void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
            protected:
                void complete();
                void setFadeSpeed(const float speed) { fadeSpeed = speed; }
                float getFadeSpeed() const { return fadeSpeed; }

                virtual void addChild(cocos2d::Node *child) override
                {
                    addChild(child, child->getLocalZOrder(), child->getTag());
                }

                virtual void addChild(cocos2d::Node *child, int localZOrder) override
                {
                    addChild(child, localZOrder, child->getTag());
                }

                virtual void addChild(cocos2d::Node *child, int localZOrder, int tag) override;

                ClippingScale9Sprite *createClip(const std::string &filename, const float width, const float height);
                ClippingScale9Sprite *createClip(const std::string &filename, const float size);
                ClippingScale9Sprite *createClipWithSpriteFrameName(const std::string &name, const float width, const float height);
                ClippingScale9Sprite *createClipWithSpriteFrameName(const std::string &name, const float size);
                ClippingSprite *createClip(const std::string &filename);
                ClippingSprite *createClipWithSpriteFrameName(const std::string &name);
                ClippingRect *createClip(const float width, const float height);
                ClippingRect *createClip(const cocos2d::Size &size) { return createClip(size.width, size.height); }

            public:
                TutorialBaseBase()
                : maskLayer(nullptr)
                , listener(nullptr)
                , completed(false)
                , locked(true)
                , touchEnabled(true)
                , fadeSpeed(0.4f)
                , seq_id(0)
                { }
                virtual ~TutorialBaseBase() { }

                virtual void onEnter() override;
                virtual void onExit() override;

                bool hasRoot() const { return getParent(); }
                bool showTutorial(TutorialPhase *phase, const std::function<bool()> &callback);
                void setTouchEnabled(const bool enable) { touchEnabled = enable; }
                void setLock(const bool lock) { locked = lock; }
                virtual void registerPhase() = 0;
            };
        };
    }
}

#endif

