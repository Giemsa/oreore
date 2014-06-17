#ifndef __OREORE_COCOS2D_PLAYSE_H__
#define __OREORE_COCOS2D_PLAYSE_H__

#include "cocos2d.h"

namespace oreore
{
    class PlaySE : public cocos2d::ActionInstant
    {
    private:
        std::string filename;
    public:
        PlaySE() { }
        virtual ~PlaySE() { }

        static PlaySE* create(const std::string &filename);
        virtual bool init(const std::string &filename);

        virtual void update(float time);

        virtual PlaySE *reverse() const override;
        virtual PlaySE *clone() const override;
    };
}

#endif
