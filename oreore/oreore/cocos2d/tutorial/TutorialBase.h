#ifndef __OREORE_COCOS2DX_TUTORIALBASE_H__
#define __OREORE_COCOS2DX_TUTORIALBASE_H__

#include "cocos2d.h"
#include "TutorialSequence.h"

namespace oreore
{
    namespace detail
    {
        template<typename T>
        class TutorialPlayInfo;
    };

    /* チュートリアルベース */
    class TutorialBase : public cocos2d::ClippingNode
    {
        template<typename T>
        friend class detail::TutorialPlayInfo;
    private:
        cocos2d::LayerColor *maskLayer;
        cocos2d::Layer *layer;
        cocos2d::EventListenerTouchOneByOne *listener;
        bool completed;
        bool locked;
        float fadeSpeed;

        bool init() override;
    protected:

        // implementation in Tutorial.h
        template<typename T>
        static TutorialSequence &addTrigger(const T trigger);

        void complete();
        void setFadeSpeed(const float speed) { fadeSpeed = speed; }
        float getFadeSpeed() const { return fadeSpeed; }

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
        virtual void addChild(cocos2d::Node *child) override;
        virtual void addChild(cocos2d::Node *child, int localZOrder) override;
        virtual void addChild(cocos2d::Node *child, int localZOrder, int tag) override;
    public:
        TutorialBase()
        : maskLayer(nullptr)
        , layer(nullptr)
        , listener(nullptr)
        , completed(false)
        , locked(true)
        , fadeSpeed(0.4f)
        { }
        virtual ~TutorialBase() { }

        virtual void onEnter() override;
        virtual void onExit() override;

        bool hasRoot() const { return getParent(); }
        bool showTutorial(const std::function<bool()> &callback);
    };

    namespace detail
    {
        struct TutorialPlayInfoBase
        {
            bool isPlayed;
            TutorialBase *instance;

            TutorialPlayInfoBase()
            : isPlayed(false), instance(nullptr)
            { }

            virtual ~TutorialPlayInfoBase()
            {
                instance->release();
            }

            virtual TutorialBase *create() const = 0;
            TutorialBase *getInstance()
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

#endif

