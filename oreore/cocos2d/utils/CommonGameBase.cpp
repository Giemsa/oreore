#include "CommonGameBase.h"
#include "Utils.h"

namespace oreore
{
    using namespace cocos2d;

    /* IDConnector */
    unsigned int IDConnector::id = 0;

    /* ScreenBase */
    ScreenBase::ScreenBase()
    {
        _scene = CCScene::create();
        _scene->retain();
        _scene->addChild(this);
    }

    ScreenBase::~ScreenBase()
    {
        _scene->release();
    }

    CCSpriteBatchNode *ScreenBase::registerSprite(const char *filename)
    {
        CCSpriteBatchNode *batch = CCSpriteBatchNode::create(filename);
        addChild(batch);
        return batch;
    }
}
