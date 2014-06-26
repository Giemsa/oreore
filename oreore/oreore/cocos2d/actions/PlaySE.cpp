#include "PlaySE.h"
#include "../../null.h"
#include "../utils/Sound.h"

namespace oreore
{
    using namespace cocos2d;
    using namespace CocosDenshion;

    PlaySE *PlaySE::create(const std::string &filename)
    {
        PlaySE *action = new PlaySE();
        if(action && action->init(filename))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return null;
    }

    bool PlaySE::init(const std::string &filename)
    {
        this->filename = filename;
        return true;
    }

    void PlaySE::update(float time)
    {
        SoundManager::getInstance().playSE(filename);
    }

    PlaySE *PlaySE::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return null;
    }

    PlaySE *PlaySE::clone() const
    {
        return PlaySE::create(filename);
    }
}

