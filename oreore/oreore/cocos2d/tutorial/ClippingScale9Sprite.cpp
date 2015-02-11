#include "ClippingScale9Sprite.h"
#include "TutorialBase.h"

namespace oreore
{
    using namespace cocos2d;

    namespace Tutorial
    {
        /* ClippingScale9Sprite */
        ClippingScale9Sprite* ClippingScale9Sprite::createWithSpriteFrameName(const std::string &name, const float width, const float height)
        {
            ClippingScale9Sprite *r = new ClippingScale9Sprite();
            if(r && r->initWithSpriteFrameName(name, width, height))
            {
                r->autorelease();
                return r;
            }

            delete r;
            return nullptr;
        }

        ClippingScale9Sprite* ClippingScale9Sprite::create(const std::string &fileName, const float width, const float height)
        {
            ClippingScale9Sprite *r = new ClippingScale9Sprite();
            if(r && r->initWithFile(fileName, width, height))
            {
                r->autorelease();
                return r;
            }

            delete r;
            return nullptr;
        }

        bool ClippingScale9Sprite::initWithSpriteFrameName(const std::string &name, const float width, const float height)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
            const float w = frame->getRect().size.width;
            const float h = frame->getRect().size.height;

            const float perWidth = w / 100.0f * width;
            const float perHeight = h / 100.0f * height;

            const Rect &capInsents = Rect(perWidth, perHeight, w - perWidth * 2, h - perHeight * 2);

            if(!Scale9Sprite::initWithSpriteFrame(frame, capInsents))
            {
                return false;
            }

            _scale9Image->setGlobalZOrder(detail::TutorialBaseBase::DefaultZOrder);
            _scale9Image->setBlendFunc({ GL_DST_COLOR, GL_ONE });
            setTouchEnabled(true);

            return true;
        }

        bool ClippingScale9Sprite::initWithFile(const std::string &fileName, const float width, const float height)
        {
            Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(fileName);
            if(!texture)
            {
                return false;
            }

            const float w = texture->getContentSize().width;
            const float h = texture->getContentSize().height;
            const float perWidth = w / 100.0f * width;
            const float perHeight = h / 100.0f * height;

            const Rect &capInsents = Rect(perWidth, perHeight, w - perWidth * 2, h - perHeight * 2);
            const Rect &capRect = Rect(0, 0, w, h);
            SpriteBatchNode *node = SpriteBatchNode::createWithTexture(texture , 9);

            if(!Scale9Sprite::initWithBatchNode(node, capRect, capInsents))
            {
                return false;
            }

            _scale9Image->setGlobalZOrder(detail::TutorialBaseBase::DefaultZOrder);
            _scale9Image->setBlendFunc({ GL_DST_COLOR, GL_ONE });
            setTouchEnabled(true);

            return true;
        }

        bool ClippingScale9Sprite::checkTouch(const cocos2d::Point &pos) const
        {
            if(hasTouchHandle())
            {
                return ClippingNode::checkTouch(pos);
            }

            return getBoundingBox().containsPoint(pos);
        }
    }
}