#include "PlaySE.h"
#include "../../null.h"
#include "../utils/Sound.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    CCPlaySE *CCPlaySE::create(const std::string &filename)
    {
        CCPlaySE *action = new CCPlaySE();
        if(action && action->init(filename))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool CCPlaySE::init(const std::string &filename)
    {
        this->filename = filename;
        return true;
    }

    void CCPlaySE::update(float time)
    {
        SoundManager::getInstance().playSE(filename);
    }

    CCPlaySE *CCPlaySE::reverse()
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    CCPlaySE *CCPlaySE::clone()
    {
        return CCPlaySE::create(filename);
    }
}
