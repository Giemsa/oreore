#include "TutorialClippingSprite.h"
#include "TutorialBase.h"

namespace oreore
{
    using namespace cocos2d;

    namespace Tutorial
    {
        /* ClippingSprite */
        ClippingSprite* ClippingSprite::createWithSpriteFrameName(const std::string &fileName, const float width, const float height)
        {
            ClippingSprite *r = new ClippingSprite();
            if(r && r->initWithSpriteFrameName(fileName, width, height))
            {
                r->autorelease();
                return r;
            }

            delete r;
            return nullptr;
        }

        ClippingSprite* ClippingSprite::create(const std::string &fileName, const float width, const float height)
        {
            ClippingSprite *r = new ClippingSprite();
            if(r && r->initWithFile(fileName, width, height))
            {
                r->autorelease();
                return r;
            }

            delete r;
            return nullptr;
        }

        bool ClippingSprite::initWithSpriteFrameName(const std::string &fileName, const float width, const float height)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
            const float w = frame->getRect().size.width;
            const float h = frame->getRect().size.height;

            const float perWidth = w / 100.0f * width;
            const float perHeight = h / 100.0f * height;

            const Rect &capInsents = Rect(perWidth, perHeight, w - perWidth * 2, h - perHeight * 2);

            if(!Scale9Sprite::initWithSpriteFrame(frame, capInsents))
            {
                return false;
            }

            _scale9Image->setGlobalZOrder(TutorialBase::DefaultZOrder);
            _scale9Image->setBlendFunc({ GL_DST_COLOR, GL_ONE });
            touchEnabled = true;

            return true;
        }

        bool ClippingSprite::initWithFile(const std::string &fileName, const float width, const float height)
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

            _scale9Image->setGlobalZOrder(TutorialBase::DefaultZOrder);
            _scale9Image->setBlendFunc({ GL_DST_COLOR, GL_ONE });
            touchEnabled = true;

            return true;
        }
    }
}