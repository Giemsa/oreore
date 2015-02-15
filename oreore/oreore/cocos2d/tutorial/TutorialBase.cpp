#include "TutorialBase.h"
#include "ClippingNode.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    namespace Tutorial
    {
        using namespace cocos2d;

        namespace detail
        {
            /* TutorialBaseBase */
            bool TutorialBaseBase::init()
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

            void TutorialBaseBase::onEnter()
            {
                Node::onEnter();
                listener = EventListenerTouchOneByOne::create();
                listener->setSwallowTouches(true);
                listener->onTouchBegan = CC_CALLBACK_2(TutorialBaseBase::onTouchBegan, this);
                listener->onTouchEnded = CC_CALLBACK_2(TutorialBaseBase::onTouchEnded, this);
                getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
            }

            void TutorialBaseBase::onExit()
            {
                getEventDispatcher()->removeEventListener(listener);
                Node::onExit();
            }

            bool TutorialBaseBase::onTouchBegan(Touch *touch, Event *event)
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
                        if(clip->checkTouch(p))
                        {
                            return false;
                        }
                    }
                }

                if(touchEnabled && completed)
                {
                    // next phase
                }

                return true;
            }

            void TutorialBaseBase::onTouchEnded(Touch *touch, Event *event)
            {
                if(locked)
                {
                    return;
                }

                /*
                if(completed)
                {
                    if(sequence)
                    {
                        sequence->proceed();
                    }
                }
                */
            }

            void TutorialBaseBase::addChild(Node *child, int localZOrder, int tag)
            {
                child->setGlobalZOrder(child->getGlobalZOrder() + DefaultZOrder + 1);
                Node::addChild(child, localZOrder, tag);
            }

            ClippingScale9Sprite *TutorialBaseBase::createClip(const std::string &filename, const float width, const float height)
            {
                ClippingScale9Sprite *sprite = ClippingScale9Sprite::create(filename, width, height);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            ClippingScale9Sprite *TutorialBaseBase::createClip(const std::string &filename, const float size)
            {
                ClippingScale9Sprite *sprite = ClippingScale9Sprite::create(filename, size);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            ClippingScale9Sprite *TutorialBaseBase::createClipWithSpriteFrameName(const std::string &name, const float width, const float height)
            {
                ClippingScale9Sprite *sprite = ClippingScale9Sprite::createWithSpriteFrameName(name, width, height);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            ClippingScale9Sprite *TutorialBaseBase::createClipWithSpriteFrameName(const std::string &name, const float size)
            {
                ClippingScale9Sprite *sprite = ClippingScale9Sprite::createWithSpriteFrameName(name, size);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            ClippingSprite *TutorialBaseBase::createClip(const std::string &filename)
            {
                ClippingSprite *sprite = ClippingSprite::create(filename);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            ClippingSprite *TutorialBaseBase::createClipWithSpriteFrameName(const std::string &name)
            {
                ClippingSprite *sprite = ClippingSprite::createWithSpriteFrameName(name);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            ClippingRect *TutorialBaseBase::createClip(const float width, const float height)
            {
                ClippingRect *sprite = ClippingRect::create(width, height);
                Node::addChild(sprite, sprite->getLocalZOrder(), sprite->getTag());
                return sprite;
            }

            void TutorialBaseBase::complete()
            {
                if(finished)
                {
                    if(getParent() && getOpacity() > 0)
                    {
                        locked = true;
                        runAction(
                            x::fadeOut(fadeSpeed) >> [this]() {
                                removeTutorial();
                                removeFromParent();
                            }
                        );
                    }
                    else
                    {
                        removeTutorial();
                        removeFromParent();
                    }
                }
                else
                {
                    completed = true;
                }
            }

            bool TutorialBaseBase::showTutorial(const std::function<bool()> &callback)
            {
                completed = false;

                if(!getParent())
                {
                    Scene *active = Director::getInstance()->getRunningScene();
                    if(active)
                    {
                        active->addChild(this);
                    }

                    locked = true;
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
}
