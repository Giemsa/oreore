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
    const Point &origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );

    closeItem->setPosition(
        Point(
            origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
            origin.y + closeItem->getContentSize().height / 2
        )
    );

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu, 1);

    auto label = LabelTTF::create("Hello World", "Arial", 24);
    label->setPosition(
        Point(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height
        )
    );

    addChild(label, 1);

    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(
        Point(
            visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y
        )
    );

    addChild(sprite, 0);

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
