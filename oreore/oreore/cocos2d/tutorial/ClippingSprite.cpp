#include "ClippingSprite.h"
#include "TutorialBase.h"

namespace oreore
{
    using namespace cocos2d;

    namespace Tutorial
    {
        /* ClippingSprite */
        ClippingSprite* ClippingSprite::createWithSpriteFrameName(const std::string &name)
        {
            ClippingSprite *r = new ClippingSprite();
            if(r && r->initWithSpriteFrameName(name))
            {
                r->autorelease();
                return r;
            }

            delete r;
            return nullptr;
        }

        ClippingSprite* ClippingSprite::create(const std::string &fileName)
        {
            ClippingSprite *r = new ClippingSprite();
            if(r && r->initWithFile(fileName))
            {
                r->autorelease();
                return r;
            }

            delete r;
            return nullptr;
        }

        bool ClippingSprite::initWithSpriteFrameName(const std::string &name)
        {
            if(!Sprite::initWithSpriteFrameName(name))
            {
                return false;
            }

            setGlobalZOrder(detail::TutorialBaseBase::DefaultZOrder);
            setBlendFunc({ GL_DST_COLOR, GL_ONE });
            setTouchEnabled(true);

            return true;
        }

        bool ClippingSprite::initWithFile(const std::string &fileName)
        {
            if(!Sprite::initWithFile(fileName))
            {
                return false;
            }

            setGlobalZOrder(detail::TutorialBaseBase::DefaultZOrder);
            setBlendFunc({ GL_DST_COLOR, GL_ONE });
            setTouchEnabled(true);

            return true;
        }
    }
}