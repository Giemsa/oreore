#ifndef __OREORE_COCOS2D_DEBUG_H__
#define __OREORE_COCOS2D_DEBUG_H__

#include "cocos2d.h"
#include <vector>
#include "../../null.h"
#include "cocos-ext.h"

namespace oreore
{
    class Debugger;
    typedef void (* DebugMenuCallback)(Debugger *);

    class DebugMenuItem : public cocos2d::CCLayerColor
    {
    private:
        DebugMenuCallback callback;
    public:
        DebugMenuItem() : callback(null) { }
        static DebugMenuItem *create(const std::string &name, const DebugMenuCallback callback, const float width);

        bool init(const std::string &name, const DebugMenuCallback callback, const float width);
        void event(const cocos2d::CCPoint &p, Debugger *debugger);
        void select(const cocos2d::CCPoint &p);
        void unselect();
    };

    class DebugMenu : public cocos2d::CCLayer
    {
    private:
        cocos2d::CCPoint bPos;
        Debugger *debugger;
    public:
        CREATE_FUNC(DebugMenu);
        
        bool init(); // override

        bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        void show();
        void hide();
        void setDebugger(Debugger *debugger) { this->debugger = debugger; }
    };

    class DebugLayer : public cocos2d::CCLayer
    {
    private:
        cocos2d::extension::CCScrollView *scrollView;
        cocos2d::extension::CCScale9Sprite *frame;
        cocos2d::CCSprite *icon;
        bool opened;
        cocos2d::CCPoint bPos;
        DebugMenu *menuLayer;
    public:
        CREATE_FUNC(DebugLayer);
        bool init(); // override
        void visit(); // override
        bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override
        void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event); // override

        void addItem(const std::string &name, const DebugMenuCallback callback);
        void setDebugger(Debugger *debugger) { menuLayer->setDebugger(debugger); }
    };

    class Debugger
    {
        friend class SceneManager;
    private:
        DebugLayer *layer;

        Debugger(DebugLayer *layer) : layer(layer) { layer->setDebugger(this); }
    public:
        void addMenuItem(const std::string &name, const DebugMenuCallback callback);
    };
}

#endif
