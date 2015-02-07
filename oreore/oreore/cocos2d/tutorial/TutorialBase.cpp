#include "TutorialBase.h"
#include "TutorialClippingRect.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    using namespace cocos2d;

    namespace
    {
        const float TutorialZOrder = 10240;
    }

    /* TutorialBase */
    bool TutorialBase::init()
    {
        if(!ClippingNode::init())
        {
            return false;
        }

        const Size &size = Director::getInstance()->getWinSize();
        maskLayer = LayerColor::create(Color4B(0, 0, 0, 0x88), size.width, size.height);
        maskLayer->setGlobalZOrder(TutorialZOrder);
        ClippingNode::addChild(maskLayer, maskLayer->getLocalZOrder(), maskLayer->getTag());

        layer = Layer::create();
        layer->setGlobalZOrder(TutorialZOrder);

        setInverted(true);
        setStencil(layer);
        setPosition(Point::ZERO);
        setAlphaThreshold(1.0f);
        setContentSize(size);
        setGlobalZOrder(TutorialZOrder);
        setVisible(false);
        setCascadeOpacityEnabled(true);

        ignoreAnchorPointForPosition(true);

        return true;
    }

    void TutorialBase::onEnter()
    {
        ClippingNode::onEnter();
        listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(TutorialBase::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(TutorialBase::onTouchEnded, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }

    void TutorialBase::onExit()
    {
        getEventDispatcher()->removeEventListener(listener);
        ClippingNode::onExit();
    }

    bool TutorialBase::onTouchBegan(Touch *touch, Event *event)
    {
        if(locked)
        {
            return true;
        }

        const Point &p = touch->getLocation();
        for(auto *node : layer->getChildren())
        {
            TutorialClippingRect *clip = dynamic_cast<TutorialClippingRect *>(node);
            if(clip && clip->isTouchEnabled() && clip->hit(p))
            {
                return false;
            }
        }

        return true;
    }

    void TutorialBase::onTouchEnded(Touch *touch, Event *event)
    {
    
    }

    void TutorialBase::complete()
    {
        completed = true;
    }

    void TutorialBase::addChild(cocos2d::Node *child)
    {
        child->setGlobalZOrder(TutorialZOrder);
        layer->addChild(child);
    }

    void TutorialBase::addChild(cocos2d::Node *child, int localZOrder)
    {
        child->setGlobalZOrder(TutorialZOrder);
        layer->addChild(child, localZOrder);
    }

    void TutorialBase::addChild(cocos2d::Node *child, int localZOrder, int tag)
    {
        child->setGlobalZOrder(TutorialZOrder);
        layer->addChild(child, localZOrder, tag);
    }

    bool TutorialBase::showTutorial(const std::function<bool()> &callback)
    {
        using namespace oreore;

        if(!getParent())
        {
            Scene *active = Director::getInstance()->getRunningScene();
            if(active)
            {
                active->addChild(this);
            }

            setVisible(true);
            setOpacity(0x00);
            runAction(
                x::fadeIn(fadeSpeed) >> [this, callback]() {
                    locked = false;
                    callback();
                }
            );

            return true;
        }

        return callback();
    }
}
