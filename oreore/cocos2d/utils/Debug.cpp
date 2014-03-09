#include "Debug.h"
#include "Utils.h"

namespace oreore
{
    using namespace cocos2d;

    /* DebugLayer */
    bool DebugLayer::init()
    {
        if(!Layer::init())
            return false;

        LabelTTF *label = LabelTTF::create("debug!!!!", "", 80.0f);
        label->setPosition(center(this));
        addChild(label);

        return true;
    }

    void DebugLayer::visit()
    {
        Layer::visit();
    }
}