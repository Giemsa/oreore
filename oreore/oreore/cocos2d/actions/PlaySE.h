#ifndef __OREORE_COCOS2D_PLAYSE_H__
#define __OREORE_COCOS2D_PLAYSE_H__

#include "cocos2d.h"
#include "../utils/Fluxion.h"

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

    // fluxion plugin
    namespace fluxion
    {
        using PlaySE = Action<oreore::PlaySE>;
    }

    namespace x
    {
        inline fluxion::PlaySE playSE(const std::string &filename)
        {
            return fluxion::PlaySE(filename);
        }
    }
}

#endif

