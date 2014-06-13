#include "Debug.h"
#include "Utils.h"
#include "./images/icon.h"
#include "./images/bg.h"

namespace oreore
{
    using namespace cocos2d;

    /* DebugMenuItem */
    DebugMenuItem *DebugMenuItem::create(const std::string &name, const std::function<void(Debugger *)> &callback, const float width)
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

    bool DebugMenuItem::init(const std::string &name, const std::function<void(Debugger *)> &callback, const float width)
    {
        if(!LayerColor::initWithColor(Color4B(0xCC, 0xCC, 0xCC, 0x80)))
            return false;

        setAnchorPoint(Point::ZERO);
        ignoreAnchorPointForPosition(false);
        setContentSize(Size(width, 100 / CC_CONTENT_SCALE_FACTOR()));

        Label *label = Label::createWithSystemFont(name, "", 40.0f);
        label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        label->setPosition(Point(10.0f / CC_CONTENT_SCALE_FACTOR(), getContentSize().height / 2));
        label->setColor(Color3B(0x55, 0x55, 0x55));
        addChild(label);

        this->callback = callback;

        return true;
    }

    void DebugMenuItem::event(const Point &p, Debugger *debugger)
    {
        if(callback && getBoundingBox().containsPoint(p))
            callback(debugger);
    }

    void DebugMenuItem::select(const cocos2d::Point &p)
    {
        if(getBoundingBox().containsPoint(p))
        {
            setOpacity(0xFF);
            setColor(Color3B(0x70, 0x70, 0x70));
        }
    }
    
    void DebugMenuItem::unselect()
    {
        setOpacity(0x80);
        setColor(Color3B(0xCC, 0xCC, 0xCC));
    }

    /* DebugMenu */
    DebugMenu::~DebugMenu()
    {
        if(listener)
            listener->release();
    }

    bool DebugMenu::init()
    {
        if(!Layer::init())
            return false;

        listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(DebugMenu::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(DebugMenu::onTouchEnded, this);
        listener->setSwallowTouches(false);
        getEventDispatcher()->addEventListenerWithFixedPriority(listener, -1001);

        return true;
    }

    bool DebugMenu::onTouchBegan(Touch* touch, Event* event)
    {
        const Point &p = convertTouchToNodeSpace(touch);
        bPos = getParent()->convertTouchToNodeSpace(touch);
        for(Node *obj : getChildren())
        {
            DebugMenuItem *item = dynamic_cast<DebugMenuItem *>(obj);
            if(item)
                item->select(p);
        }
        return true;
    }

    void DebugMenu::onTouchEnded(Touch *touch, Event* event)
    {
        const Point &p = convertTouchToNodeSpace(touch);
        for(Node *obj : getChildren())
        {
            DebugMenuItem *item = dynamic_cast<DebugMenuItem *>(obj);
            if(item)
                item->unselect();
        }
        
        if(getParent()->convertTouchToNodeSpace(touch).getDistance(bPos) < 40.0f)
        {
            for(Node *obj : getChildren())
            {
                DebugMenuItem *item = dynamic_cast<DebugMenuItem *>(obj);
                if(item)
                    item->event(p, debugger);
            }
        }
    }

    /* DebugLayer */
    DebugLayer::~DebugLayer()
    {
        layerEventListener->release();
        scrollViewEventListener->release();
    }

    bool DebugLayer::init()
    {
        using namespace cocos2d::extension;

        if(!Layer::init())
            return false;

        opened = false;
        {
            Image *img = new Image();
            img->initWithImageData(images::bg, sizeof(images::bg));

            Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(img, "_oreore_debugger_icon");
            delete img;
            
            SpriteFrame *frm = SpriteFrame::createWithTexture(
                tex,
                Rect(
                    0, 0,
                    tex->getPixelsWide() / CC_CONTENT_SCALE_FACTOR(), tex->getPixelsHigh() / CC_CONTENT_SCALE_FACTOR()
                )
            );
            frame = Scale9Sprite::createWithSpriteFrame(
                frm,
                Rect(
                    10 / CC_CONTENT_SCALE_FACTOR(),
                    10 / CC_CONTENT_SCALE_FACTOR(),
                    300 / CC_CONTENT_SCALE_FACTOR(),
                    460 / CC_CONTENT_SCALE_FACTOR()
                )
            );
            frame->setAnchorPoint(Point(1.0f, 1.0f));
            frame->setPosition(oreore::percent(this, 100.0f, 90.0f) + Point(10.0f / CC_CONTENT_SCALE_FACTOR(), 0.0f));
            frame->setContentSize(Size(102 / CC_CONTENT_SCALE_FACTOR(), 92 / CC_CONTENT_SCALE_FACTOR()));
            addChild(frame);
        }

        {
            Image *img = new Image();
            img->initWithImageData(images::icon, sizeof(images::icon));

            Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(img, "_oreore_debugger_bg");
            delete img;

            icon = Sprite::createWithTexture(tex);
            icon->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
            icon->setPosition(Point(10 / CC_CONTENT_SCALE_FACTOR(), 82 / CC_CONTENT_SCALE_FACTOR()));
            frame->addChild(icon);
        }

        scrollView = ScrollView::create();
        scrollView->setAnchorPoint(Point::ZERO);
        scrollView->setPosition(Point(10.0f / CC_CONTENT_SCALE_FACTOR(), 10.0f / CC_CONTENT_SCALE_FACTOR()));
        scrollView->setDirection(ScrollView::Direction::VERTICAL);
        frame->addChild(scrollView);

        menuLayer = DebugMenu::create();
        menuLayer->setContentSize(Size(getContentSize().width * 0.8f, 0));
        scrollView->setContainer(menuLayer);
        scrollView->setContentSize(menuLayer->getContentSize());
        scrollView->setViewSize(getContentSize() * 0.8f - Size(20 / CC_CONTENT_SCALE_FACTOR(), 102 / CC_CONTENT_SCALE_FACTOR()));
        scrollView->setVisible(false);

        // this
        layerEventListener = EventListenerTouchOneByOne::create();
        layerEventListener->onTouchBegan = CC_CALLBACK_2(DebugLayer::onTouchBegan, this);
        layerEventListener->setSwallowTouches(true);
        layerEventListener->retain();
        getEventDispatcher()->addEventListenerWithFixedPriority(layerEventListener, -999);

        // scroll view
        scrollView->setTouchEnabled(false);
        scrollViewEventListener = EventListenerTouchOneByOne::create();
        scrollViewEventListener->onTouchBegan = CC_CALLBACK_2(ScrollView::onTouchBegan, scrollView);
        scrollViewEventListener->onTouchMoved = CC_CALLBACK_2(ScrollView::onTouchMoved, scrollView);
        scrollViewEventListener->onTouchEnded = CC_CALLBACK_2(ScrollView::onTouchEnded, scrollView);
        scrollViewEventListener->onTouchCancelled = CC_CALLBACK_2(ScrollView::onTouchCancelled, scrollView);
        scrollViewEventListener->retain();
       
        scrollView->getEventDispatcher()->addEventListenerWithFixedPriority(scrollViewEventListener, -1000);
        
        onEnter();
        return true;
    }

    void DebugLayer::addItem(const std::string &name, const std::function<void(Debugger *)> &callback)
    {
        for(Node *item : menuLayer->getChildren())
            item->setPositionY(item->getPositionY() + item->getContentSize().height);

        DebugMenuItem *item = DebugMenuItem::create(name, callback, scrollView->getContentSize().width);

        if(menuLayer->getChildrenCount() > 0)
        {
            DrawNode *line = DrawNode::create();
            line->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
            line->drawSegment(Point(0.0f, item->getContentSize().height), Point(item->getContentSize()), 0.5f, Color4F(0.3f, 0.3f, 0.3f, 0.9f));
            item->addChild(line);
        }
        
        scrollView->getContainer()->addChild(item);
        scrollView->setContentSize(scrollView->getContentSize() + Size(0.0f, item->getContentSize().height));
    }


    bool DebugLayer::onTouchBegan(Touch* touch, Event* event)
    {
        const Point &p = convertTouchToNodeSpace(touch);
        if(frame->getBoundingBox().containsPoint(p))
        {
            const Point &ip = frame->convertToNodeSpace(touch->getLocation());
            if(!scrollView->getBoundingBox().containsPoint(ip) || !scrollView->isVisible())
            {
                if(opened)
                {
                    opened = false;
                    frame->setContentSize(Size(102 / CC_CONTENT_SCALE_FACTOR(), 92 / CC_CONTENT_SCALE_FACTOR()));
                    icon->setPosition(Point(10 / CC_CONTENT_SCALE_FACTOR(), 82 / CC_CONTENT_SCALE_FACTOR()));
                    scrollView->setVisible(false);
                }
                else
                {
                    opened = true;
                    frame->setContentSize(getContentSize() * 0.8f);
                    icon->setPosition(Point(10 / CC_CONTENT_SCALE_FACTOR(), frame->getContentSize().height - 10 / CC_CONTENT_SCALE_FACTOR()));
                    scrollView->setVisible(true);
                }
                bPos = p;
            }
            return true;
        }
        return false;
    }

    void DebugLayer::onTouchEnded(Touch *touch, Event* event)
    {
    }

    void DebugLayer::onTouchMoved(Touch *touch, Event* event)
    {
    }

    void DebugLayer::setTouchEnabled(const bool enable)
    {
        if(enable)
        {
            if(layerEventListener->getReferenceCount() < 3)
            {
                getEventDispatcher()->addEventListenerWithFixedPriority(layerEventListener, -999);
                scrollView->setTouchEnabled(false);
                getEventDispatcher()->addEventListenerWithFixedPriority(scrollViewEventListener, -1000);
            }
        }
        else
        {
            getEventDispatcher()->removeEventListener(layerEventListener);
            getEventDispatcher()->removeEventListener(scrollViewEventListener);
        }
    }

    
    /* Debugger */
    void Debugger::addMenuItem(const std::string &name, const std::function<void(Debugger *)> &callback)
    {
        layer->addItem(name, callback);
    }
}