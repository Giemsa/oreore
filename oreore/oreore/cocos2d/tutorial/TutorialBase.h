#ifndef __OREORE_COCOS2DX_TUTORIALBASE_H__
#define __OREORE_COCOS2DX_TUTORIALBASE_H__

#include "cocos2d.h"
#include "TutorialSequence.h"
#include "ClippingSprite.h"
#include "ClippingScale9Sprite.h"
#include "ClippingRect.h"

namespace oreore
{
    namespace Tutorial
    {
        namespace detail
        {
            template<typename T>
            class TutorialPlayInfo;

            class TutorialBaseBase : public cocos2d::Node
            {
                template<typename T>
                friend class detail::TutorialPlayInfo;
            public:
                static constexpr float DefaultZOrder = 10240.0f;
            private:
                cocos2d::LayerColor *maskLayer;
                cocos2d::EventListenerTouchOneByOne *listener;
                TutorialSequence *sequence;
                bool completed;
                bool locked;
                float fadeSpeed;

                bool init() override;
            protected:
                void complete();
                void setFadeSpeed(const float speed) { fadeSpeed = speed; }
                float getFadeSpeed() const { return fadeSpeed; }

                bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
                void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

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
                , fadeSpeed(0.4f)
                , sequence(nullptr)
                { }
                virtual ~TutorialBaseBase() { }

                virtual void onEnter() override;
                virtual void onExit() override;

                bool hasRoot() const { return getParent(); }
                bool showTutorial(TutorialSequence *seq, const std::function<bool()> &callback);
            };
        };


        namespace detail
        {
            struct TutorialPlayInfoBase
            {
                bool isPlayed;
                detail::TutorialBaseBase *instance;

                TutorialPlayInfoBase()
                : isPlayed(false), instance(nullptr)
                { }

                virtual ~TutorialPlayInfoBase()
                {
                    instance->release();
                }

                virtual detail::TutorialBaseBase *create() const = 0;
                detail::TutorialBaseBase *getInstance()
                {
                    if(!instance)
                    {
                        instance = create();
                    }

                    return instance;
                }
            };

            template<typename T>
            class TutorialPlayInfo : public TutorialPlayInfoBase
            {
                T *create() const override
                {
                    T * r = new T();
                    if(r && r->init())
                    {
                        r->autorelease();
                        r->retain();
                        return r;
                    }

                    return nullptr;
                }
            };
        }
    }
}

#endif

