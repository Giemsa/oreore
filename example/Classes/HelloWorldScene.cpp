#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace oreore;

bool HelloWorld::init()
{
    if(!SceneBase::init())
    {
        return false;
    }

    const Size &visibleSize = Director::getInstance()->getVisibleSize();

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );

    closeItem->setPosition(
        Point(
            visibleSize.width - closeItem->getContentSize().width / 2,
            closeItem->getContentSize().height / 2
        )
    );

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu, 1);

    auto label = Label::createWithSystemFont("Hello World", "Arial", 20);
    label->setPosition(
        Point(
            visibleSize.width / 2,
            visibleSize.height - label->getContentSize().height
        )
    );

    addChild(label, 1);

    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(center(this));

    addChild(sprite, 0);

    // generate action sequence with fluxion
    sprite->runAction(
        (
            x::moveBy(0.5f, -200, -200) >>
            x::moveBy(1.0f, 0, 400) * x::ease::elasticOut(0.5f) >>
            x::moveBy(1.0f, 400, -400) >>
            x::moveBy(1.0f, 0, 400) * x::ease::elasticOut(0.5f) >>
            x::moveBy(0.5f, -200, -200)
        ) * x::inf()
    );

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
