#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace oreore;

/* sample json structure */
class SaveData final : public Step::JSON
{
private:
    bool serialize(picojson::value &value) const override
    {
        picojson::object obj;
        obj["name"] = picojson::value(name);
        obj["score"] = picojson::value(static_cast<double>(score));

        value = picojson::value(obj);
        return true;
    }

    bool deserialize(const picojson::value &value) override
    {
        if(!value.is<picojson::object>())
        {
            return false;
        }

        {
            const picojson::value &v = value.get("name");
            if(!v.is<std::string>())
            {
                return false;
            }

            name = v.get<std::string>();
        }

        {
            const picojson::value &v = value.get("score");
            if(!v.is<double>())
            {
                return false;
            }

            score = static_cast<int>(v.get<double>());
        }

        return true;
    }

public:
    std::string name;
    int score;

    SaveData()
    : name(""), score(0)
    { }

    SaveData(const std::string &name, const int score)
    : name(name), score(score)
    { }

    ~SaveData() { }

};



/* sample code */
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

    // Step example
    Step::Encrypt::Blowfish bf("sample key");
    SaveData data("oreore", 123456);
    SaveData rd;
    std::string json;

    {
        Step::ProcessHolder h = Step::Serializer(data) >> Step::Encrypter(bf) >> Step::StringStorage(json);
        const bool r = h.start();

        std::cout << "convert to json: " << std::boolalpha << r << std::endl;
        std::cout << "data: " << json << std::endl;
    }

    {
        const bool r = Step::StringStorage(json) >> Step::Decrypter(bf) >> Step::Serializer(rd);

        std::cout << "convert from json: " << std::boolalpha << r << std::endl;
        std::cout << "name: " << rd.name << ", score: " << rd.score << std::endl;
    }

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
