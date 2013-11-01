#include "CommonGameBase.h"
#include "Utils.h"

namespace CommonGameBase
{
	using namespace cocos2d;
    
	/* IDConnector */
	unsigned int IDConnector::id = 0;

	/* ScreenBase */
	ScreenBase::ScreenBase()
	{
		_scene = cocos2d::Scene::create();
		_scene->addChild(this);
	}
	
	ScreenBase::~ScreenBase()
	{
	}
	
	
	SpriteBatchNode *ScreenBase::registerSprite(const char *filename)
	{
		SpriteBatchNode *batch = SpriteBatchNode::create(filename);
		addChild(batch);
		return batch;
	}
}
