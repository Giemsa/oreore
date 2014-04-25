#ifndef __OREORE_COCOS2D_DEBUG_H__
#define __OREORE_COCOS2D_DEBUG_H__

#include "cocos2d.h"
#include <vector>
#include <functional>
#include "../../null.h"
#include "extensions/cocos-ext.h"

namespace oreore
{
    class Debugger;

    class DebugMenuItem : public cocos2d::LayerColor
    {
    private:
        std::function<void(Debugger *)> callback;
    protected:
        DebugMenuItem() : callback(null) { }
        bool init(const std::string &name, const std::function<void(Debugger *)> &callback, const float width);
    public:
        static DebugMenuItem *create(const std::string &name, const std::function<void(Debugger *)> &callback, const float width);

        void event(const cocos2d::Point &p, Debugger *debugger);
        void select(const cocos2d::Point &p);
        void unselect();
    };

    class DebugMenu : public cocos2d::Layer
    {
    private:
        cocos2d::Point bPos;
        Debugger *debugger;
        cocos2d::EventListenerTouchOneByOne *listener;
    protected:
        bool init(); // override
    public:
        CREATE_FUNC(DebugMenu);
        virtual ~DebugMenu();

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
        void setDebugger(Debugger *debugger) { this->debugger = debugger; }
    };

    class DebugLayer : public cocos2d::Layer
    {
    private:
        cocos2d::extension::ScrollView *scrollView;
        cocos2d::extension::Scale9Sprite *frame;
        cocos2d::Sprite *icon;
        bool opened;
        cocos2d::Point bPos;
        DebugMenu *menuLayer;
        cocos2d::EventListenerTouchOneByOne *layerEventListener, *scrollViewEventListener;

    protected:
        bool init() override;
    public:
        CREATE_FUNC(DebugLayer);

        virtual ~DebugLayer();

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
        void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;

        void addItem(const std::string &name, const std::function<void(Debugger *)> &callback);
        void setDebugger(Debugger *debugger) { menuLayer->setDebugger(debugger); }

        void setTouchEnabled(const bool enable);
    };

    class Debugger
    {
        friend class SceneManager;
    private:
        DebugLayer *layer;

        Debugger(DebugLayer *layer) : layer(layer) { layer->setDebugger(this); }
    public:
        void addMenuItem(const std::string &name, const std::function<void(Debugger *)> &callback);
    };
}

#endif
