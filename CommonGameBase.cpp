#include "CommonGameBase.h"
#include "Utils.h"

namespace CommonGameBase
{
	using namespace cocos2d;
	
    /* NumberSprite */
    NumberSprite::NumberSprite() : format(null)
    {
    
    }
    
    NumberSprite::~NumberSprite()
    {
    
    }
    
    bool NumberSprite::init()
    {
        if(!Sprite::init())
            return false;
        return true;
    }

    void NumberSprite::setNumberTexture(cocos2d::Texture2D *tex, const Rect &rect)
    {
        setTexture(tex);
        this->rect = rect;
    }
    
    void NumberSprite::setNumber(const int n)
    {
        char buf[Utils::digits<int>() + 1];
        const int len = sprintf(buf, format, n);
        for(int i = 0; i < len; i++)
        {
            
        }
    }

    void NumberSprite::setReed(const int reed, bool zerofill)
    {
        format = static_cast<char *>(realloc(format, 6 + Utils::digits(reed)));
        sprintf(format, "%%%c%dd", (zerofill ? '0' : ' '), reed);
    }
    
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
