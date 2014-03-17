#ifndef __OREORE_COCOS2D_PLAYSE_H__
#define __OREORE_COCOS2D_PLAYSE_H__

#include "cocos2d.h"

namespace oreore
{
    class CCPlaySE : public cocos2d::CCActionInstant
    {
    private:
        std::string filename;
    public:
        CCPlaySE() { }
        virtual ~CCPlaySE() { }

        static CCPlaySE* create(const std::string &filename);
        virtual bool init(const std::string &filename);

        virtual void update(float time);

        virtual CCPlaySE *reverse(); // override
        virtual CCPlaySE *clone();
    };
}

#endif
