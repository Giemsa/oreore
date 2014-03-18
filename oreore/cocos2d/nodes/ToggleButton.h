#ifndef __OREORE_COCOS2D_TOGGLEBUTTON_H__
#define __OREORE_COCOS2D_TOGGLEBUTTON_H__

#include "cocos2d.h"
#include "../utils/Utils.h"
#include "../../null.h"

namespace oreore
{
    class CCToggleButton :
        public cocos2d::CCSprite,
        public cocos2d::CCTargetedTouchDelegate
    {
    private:
        cocos2d::SEL_MenuHandler selector;
        cocos2d::CCObject *target;
        cocos2d::CCPoint bpos;
        bool touchEnabled, toggled;
        cocos2d::CCSpriteFrame *onFrm, *offFrm;

        void _init();
        void fixSize();

        virtual cocos2d::CCFiniteTimeAction *action();
        void toggleAndAction();
    public:
        CREATE_FUNC(CCToggleButton);

        virtual ~CCToggleButton() { }

        static CCToggleButton *create(const std::string &offBtn, const std::string &onBtn);
        static CCToggleButton *createWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn);

        bool init(); //override
        bool init(const std::string &offBtn, const std::string &onBtn);
        bool initWithSpriteFrameName(const std::string &offBtn, const std::string &onBtn);

        virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override

        virtual void setTouchEnabled(const bool enable);
        virtual bool isTouchEnabled() const;

        virtual void onEnter(); // override
        virtual void onExit(); // override

        void toggleOn();
        void toggleOff();
        void toggle();
        void toggle(const bool v);
        inline bool isToggled() const { return toggled; }
        void setTappedEvent(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler callback);

    };

    template<typename T>
    class CCExtendableToggleButton : public CCToggleButton
    {
    public:
        CREATE_FUNC(T);

        virtual ~CCExtendableToggleButton() { }

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
