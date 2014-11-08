#ifndef __OREORE_COCOS2D_CIRCLEMOVETO_H__
#define __OREORE_COCOS2D_CIRCLEMOVETO_H__

#include "cocos2d.h"
#include "CircleMoveBy.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    class CircleMoveTo : public CircleMoveBy
    {
    private:
    public:
        CircleMoveTo() { }
        virtual ~CircleMoveTo() { }

        static CircleMoveTo* create(const float duration, const cocos2d::Point &center, const float deg);
        bool initWithDuration(const float duration, const cocos2d::Point &center, const float deg);

        virtual void startWithTarget(cocos2d::Node *pTarget) override;

        virtual CircleMoveTo *reverse() const override;
        virtual CircleMoveTo *clone() const override;
    };

    // fluxion plugin
    namespace fluxion
    {
        using CircleMoveTo = ActionInterval<oreore::CircleMoveTo>;
    }

    namespace x
    {
        inline fluxion::CircleMoveTo circleMoveTo(
            const float duration,
            const cocos2d::Point &center,
            const float deg
        )
        {
            return fluxion::CircleMoveTo(duration, center, deg);
        }
    }
}

#endif

