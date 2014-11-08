#ifndef __OREORE_COCOS2D_CIRCLEMOVEBY_H__
#define __OREORE_COCOS2D_CIRCLEMOVEBY_H__

#include "cocos2d.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    class CircleMoveBy : public cocos2d::ActionInterval
    {
    protected:
        cocos2d::Point center, pos, cpos;
        float deg, rad;
    public:
        CircleMoveBy() { }
        virtual ~CircleMoveBy() { }

        static CircleMoveBy* create(const float duration, const cocos2d::Point &center, const float deg);
        bool initWithDuration(const float duration, const cocos2d::Point &center, const float deg);

        virtual void startWithTarget(cocos2d::Node *pTarget) override;
        virtual void update(float time) override;
        virtual void stop(void) override;

        virtual CircleMoveBy *reverse() const override;
        virtual CircleMoveBy *clone() const override;
    };

    // fluxion plugin
    namespace fluxion
    {
        using CircleMoveBy = ActionInterval<oreore::CircleMoveBy>;
    }

    namespace x
    {
        inline fluxion::CircleMoveBy circleMoveBy(
            const float duration,
            const cocos2d::Point &center,
            const float deg
        )
        {
            return fluxion::CircleMoveBy(duration, center, deg);
        }
    }
}

#endif

