#ifndef __OREORE_COCOS2D_TOGGLEBUTTON_H__
#define __OREORE_COCOS2D_TOGGLEBUTTON_H__

#include <functional>
#include "cocos2d.h"
#include "../utils/Utils.h"
#include "../../null.h"

namespace oreore
{
    class ToggleButton : public cocos2d::Sprite
    {
    private:
        cocos2d::ccMenuCallback callback;
        cocos2d::Point bpos;
        bool touchEnabled, toggled;
        cocos2d::SpriteFrame *onFrm, *offFrm;
        cocos2d::EventListenerTouchOneByOne *listener;

        void _init();
        void fixSize();

        virtual cocos2d::FiniteTimeAction *action();
        void toggleAndAction();

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    protected:
        virtual ~ToggleButton() { }

        bool init() override;
        bool init(const std::string &offBtn, const std::string &onBtn);
        bool initWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn);
    public:
        CREATE_FUNC(ToggleButton);

        static ToggleButton *create(const std::string &offBtn, const std::string &onBtn);
        static ToggleButton *createWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn);


        virtual void setTouchEnabled(const bool enable);
        virtual bool isTouchEnabled() const;

        virtual void onEnter() override;
        virtual void onExit() override;

        void toggleOn();
        void toggleOff();
        void toggle();
        inline bool isToggled() const { return toggled; }
        void setTappedEvent(const cocos2d::ccMenuCallback &callback);

    };

    template<typename T>
    class ExtendableToggleButton : public ToggleButton
    {
    public:
        CREATE_FUNC(T);

        virtual ~ExtendableToggleButton() { }

        static T *create(const std::string &offBtn, const std::string &onBtn)
        {
            T *r = new T();
            if(r && r->init(offBtn, onBtn))
            {
                r->autorelease();
                return r;
            }
            delete r;
            return null;
        }

        static T *createWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn)
        {
            T *r = new T();
            if(r && r->initWithSpriteFrameName(offBtn, onBtn))
            {
                r->autorelease();
                return r;
            }
            delete r;
            return null;
        }
    };
}
#endif
