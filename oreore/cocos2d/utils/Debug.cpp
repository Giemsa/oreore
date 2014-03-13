#include "Debug.h"
#include "Utils.h"
#include "./images/icon.h"
#include "./images/bg.h"

namespace oreore
{
    using namespace cocos2d;

    /* DebugMenuItem */
    DebugMenuItem *DebugMenuItem::create(const std::string &name, const DebugMenuCallback callback, const float width)
    {
        DebugMenuItem *r = new DebugMenuItem();
        if(r && r->init(name, callback, width))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    bool DebugMenuItem::init(const std::string &name, const DebugMenuCallback callback, const float width)
    {
        if(!CCLayerColor::initWithColor(ccc4(0xCC, 0xCC, 0xCC, 0x80)))
            return false;

        setAnchorPoint(CCPointZero);
        ignoreAnchorPointForPosition(false);
        setContentSize(CCSize(width, 100 / CC_CONTENT_SCALE_FACTOR()));

        CCLabelTTF *label = CCLabelTTF::create(name.c_str(), "", 40.0f);
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setPosition(ccp(10.0f / CC_CONTENT_SCALE_FACTOR(), getContentSize().height / 2));
        label->setColor(ccc3(0x55, 0x55, 0x55));
        addChild(label);

        this->callback = callback;

        return true;
    }

    void DebugMenuItem::event(const CCPoint &p, Debugger *debugger)
    {
        if(callback && boundingBox().containsPoint(p))
            callback(debugger);
    }

    void DebugMenuItem::select(const cocos2d::CCPoint &p)
    {
        if(boundingBox().containsPoint(p))
        {
            setOpacity(0xFF);
            setColor(ccc3(0x70, 0x70, 0x70));
        }
    }
    
    void DebugMenuItem::unselect()
    {
        setOpacity(0x80);
        setColor(ccc3(0xCC, 0xCC, 0xCC));
    }

    /* DebugMenu */
    bool DebugMenu::init()
    {
        if(!CCLayer::init())
            return false;

        return true;
    }

    bool DebugMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
    {
        const CCPoint &p = convertTouchToNodeSpace(touch);
        bPos = getParent()->convertTouchToNodeSpace(touch);
        CCObject *obj;
        CCARRAY_FOREACH(getChildren(), obj)
        {
            DebugMenuItem *item = dynamic_cast<DebugMenuItem *>(obj);
            if(item)
                item->select(p);
        }
        return true;
    }

    void DebugMenu::ccTouchEnded(CCTouch *touch, CCEvent* event)
    {
        const CCPoint &p = convertTouchToNodeSpace(touch);
        CCObject *obj;
        CCARRAY_FOREACH(getChildren(), obj)
        {
            DebugMenuItem *item = dynamic_cast<DebugMenuItem *>(obj);
            if(item)
                item->unselect();
        }
        
        if(getParent()->convertTouchToNodeSpace(touch).getDistance(bPos) < 40.0f)
        {
            CCObject *obj;
            CCARRAY_FOREACH(getChildren(), obj)
            {
                DebugMenuItem *item = dynamic_cast<DebugMenuItem *>(obj);
                if(item)
                    item->event(p, debugger);
            }
        }
    }

    void DebugMenu::ccTouchMoved(CCTouch *touch, CCEvent* event)
    {
    }

    void DebugMenu::show()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1001, false);
    }

    void DebugMenu::hide()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    }

    /* DebugLayer */
    bool DebugLayer::init()
    {
        using namespace cocos2d::extension;

        if(!CCLayer::init())
            return false;

        opened = false;

        {

            CCImage *img = new CCImage();
            img->initWithImageData(images::bg, sizeof(images::bg), CCImage::kFmtPng);

            CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addUIImage(img, null);
            delete img;
            
            CCSpriteFrame *frm = CCSpriteFrame::createWithTexture(
                tex,
                CCRect(
                    0, 0,
                    tex->getPixelsWide() / CC_CONTENT_SCALE_FACTOR(), tex->getPixelsHigh() / CC_CONTENT_SCALE_FACTOR()
                )
            );
            frame = CCScale9Sprite::createWithSpriteFrame(
                frm,
                CCRect(
                    10 / CC_CONTENT_SCALE_FACTOR(),
                    10 / CC_CONTENT_SCALE_FACTOR(),
                    300 / CC_CONTENT_SCALE_FACTOR(),
                    460 / CC_CONTENT_SCALE_FACTOR()
                )
            );
            frame->setAnchorPoint(ccp(1.0f, 1.0f));
            frame->setPosition(oreore::percent(this, 100.0f, 90.0f) + ccp(10.0f / CC_CONTENT_SCALE_FACTOR(), 0.0f));
            frame->setContentSize(CCSize(102 / CC_CONTENT_SCALE_FACTOR(), 92 / CC_CONTENT_SCALE_FACTOR()));
            addChild(frame);
        }

        {
            CCImage *img = new CCImage();
            img->initWithImageData(images::icon, sizeof(images::icon), CCImage::kFmtPng);

            CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addUIImage(img, null);
            delete img;

            icon = CCSprite::createWithTexture(tex);
            icon->setAnchorPoint(ccp(0.0f, 1.0f));
            icon->setPosition(ccp(10 / CC_CONTENT_SCALE_FACTOR(), 82 / CC_CONTENT_SCALE_FACTOR()));
            frame->addChild(icon);
        }

        scrollView = CCScrollView::create();
        scrollView->setAnchorPoint(CCPointZero);
        scrollView->setPosition(ccp(10.0f / CC_CONTENT_SCALE_FACTOR(), 10.0f / CC_CONTENT_SCALE_FACTOR()));
        scrollView->setDirection(kCCScrollViewDirectionVertical);
        frame->addChild(scrollView);

        menuLayer = DebugMenu::create();
        menuLayer->setContentSize(CCSize(getContentSize().width * 0.8f, 0));
        scrollView->setContainer(menuLayer);
        scrollView->setContentSize(menuLayer->getContentSize());
        scrollView->setViewSize(getContentSize() * 0.8f - CCSize(20 / CC_CONTENT_SCALE_FACTOR(), 102 / CC_CONTENT_SCALE_FACTOR()));
        scrollView->setVisible(false);

        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -999, true);
        scrollView->setTouchEnabled(false);
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(scrollView, -1000, true);
        
        onEnter();
        return true;
    }

    void DebugLayer::visit()
    {
        CCLayer::visit();
    }

    void DebugLayer::addItem(const std::string &name, const DebugMenuCallback callback)
    {
        CCObject *obj;
        CCARRAY_FOREACH(menuLayer->getChildren(), obj)
        {
            CCNode *item = static_cast<CCNode *>(obj);
            item->setPositionY(item->getPositionY() + item->getContentSize().height);
        }

        DebugMenuItem *item = DebugMenuItem::create(name, callback, scrollView->getContentSize().width);

        if(menuLayer->getChildrenCount() > 0)
        {
            CCDrawNode *line = CCDrawNode::create();
            line->setAnchorPoint(ccp(0.0f, 1.0f));
            line->drawSegment(ccp(0.0f, item->getContentSize().height), item->getContentSize(), 0.5f, ccc4f(0.3f, 0.3f, 0.3f, 0.9f));
            item->addChild(line);
        }
        
        scrollView->getContainer()->addChild(item);
        scrollView->setContentSize(scrollView->getContentSize() + CCSize(0.0f, item->getContentSize().height));
    }


    bool DebugLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
    {
        const CCPoint &p = convertTouchToNodeSpace(touch);
        if(frame->boundingBox().containsPoint(p))
        {
            if(!scrollView->boundingBox().containsPoint(p))
            {
                if(opened)
                {
                    opened = false;
                    frame->setContentSize(CCSize(102 / CC_CONTENT_SCALE_FACTOR(), 92 / CC_CONTENT_SCALE_FACTOR()));
                    icon->setPosition(ccp(10 / CC_CONTENT_SCALE_FACTOR(), 82 / CC_CONTENT_SCALE_FACTOR()));
                    scrollView->setVisible(false);
                    menuLayer->hide();
                }
                else
                {
                    opened = true;
                    frame->setContentSize(getContentSize() * 0.8f);
                    icon->setPosition(ccp(10 / CC_CONTENT_SCALE_FACTOR(), frame->getContentSize().height - 10 / CC_CONTENT_SCALE_FACTOR()));
                    scrollView->setVisible(true);
                    menuLayer->show();
                }
                bPos = p;
            }
            return true;
        }
        return false;
    }

    void DebugLayer::ccTouchEnded(CCTouch *touch, CCEvent* event)
    {
    }

    void DebugLayer::ccTouchMoved(CCTouch *touch, CCEvent* event)
    {
    }


    /* Debugger */
    void Debugger::addMenuItem(const std::string &name, const DebugMenuCallback callback)
    {
        layer->addItem(name, callback);
    }
}