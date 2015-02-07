#include "TutorialBase.h"
#include "ClippingNode.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    namespace Tutorial
    {
        using namespace cocos2d;

        /* TutorialBase */
        bool TutorialBase::init()
        {
            if(!Node::init())
            {
                return false;
            }

            const Size &size = Director::getInstance()->getWinSize();
            maskLayer = LayerColor::create(Color4B(0, 0, 0, 0x88), size.width, size.height);
            maskLayer->setGlobalZOrder(DefaultZOrder);
            Node::addChild(maskLayer, maskLayer->getLocalZOrder(), maskLayer->getTag());

            setPosition(Point::ZERO);
            setContentSize(size);
            ignoreAnchorPointForPosition(true);

            setVisible(false);
            setCascadeOpacityEnabled(true);
            return true;
        }

        void TutorialBase::onEnter()
        {
            Node::onEnter();
            listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = CC_CALLBACK_2(TutorialBase::onTouchBegan, this);
            listener->onTouchEnded = CC_CALLBACK_2(TutorialBase::onTouchEnded, this);
            getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        }

        void TutorialBase::onExit()
        {
            getEventDispatcher()->removeEventListener(listener);
            Node::onExit();
        }

        bool TutorialBase::onTouchBegan(Touch *touch, Event *event)
        {
            if(locked)
            {
                return true;
            }

            const Point &p = touch->getLocation();
            for(auto *node : getChildren())
            {
                if(node == maskLayer)
                {
                    continue;
                }

                ClippingNode *clip = dynamic_cast<ClippingNode *>(node);
                if(clip)
                {
                    if(clip->isTouchEnabled() && node->getBoundingBox().containsPoint(p))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        void TutorialBase::onTouchEnded(Touch *touch, Event *event)
        {
        
        }

        void TutorialBase::addChild(Node *child, int localZOrder, int tag)
        {
            child->setGlobalZOrder(child->getGlobalZOrder() + DefaultZOrder + 1);
            Node::addChild(child, localZOrder, tag);
        }

        ClippingScale9Sprite *TutorialBase::createClip(const std::string &filename, const float width, const float height)
        {
            ClippingScale9Sprite *sprite = ClippingScale9Sprite::create(filename, width, height);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        ClippingScale9Sprite *TutorialBase::createClip(const std::string &filename, const float size)
        {
            ClippingScale9Sprite *sprite = ClippingScale9Sprite::create(filename, size);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        ClippingScale9Sprite *TutorialBase::createClipWithSpriteFrameName(const std::string &name, const float width, const float height)
        {
            ClippingScale9Sprite *sprite = ClippingScale9Sprite::createWithSpriteFrameName(name, width, height);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        ClippingScale9Sprite *TutorialBase::createClipWithSpriteFrameName(const std::string &name, const float size)
        {
            ClippingScale9Sprite *sprite = ClippingScale9Sprite::createWithSpriteFrameName(name, size);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        ClippingSprite *TutorialBase::createClip(const std::string &filename)
        {
            ClippingSprite *sprite = ClippingSprite::create(filename);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        ClippingSprite *TutorialBase::createClipWithSpriteFrameName(const std::string &name)
        {
            ClippingSprite *sprite = ClippingSprite::createWithSpriteFrameName(name);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        ClippingRect *TutorialBase::createClip(const float width, const float height)
        {
            ClippingRect *sprite = ClippingRect::create(width, height);
            Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
            return sprite;
        }

        void TutorialBase::complete()
        {
            completed = true;
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
}
