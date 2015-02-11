#ifndef __OREORE_COCOS2D_RESIZEBY_H__
#define __OREORE_COCOS2D_RESIZEBY_H__

#include "cocos2d.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    class ResizeBy : public cocos2d::ActionInterval
    {
    protected:
        cocos2d::Size size_, baseSize;
    public:
        ResizeBy() { }
        virtual ~ResizeBy() { }

        static ResizeBy* create(const float duration, const cocos2d::Size &size);
        static ResizeBy* create(const float duration, const float width, const float height)
        {
            return create(duration, cocos2d::Size(width, height));
        }

        virtual bool initWithDuration(const float duration, const cocos2d::Size &size);
        virtual void startWithTarget(cocos2d::Node *pTarget);
        virtual void update(float time) override;

        virtual ResizeBy *reverse() const override;
        virtual ResizeBy *clone() const override;
    };

    // fluxion plugin
    namespace fluxion
    {
        using ResizeBy = ActionInterval<oreore::ResizeBy>;
    }

    namespace x
    {
        inline fluxion::ResizeBy resizeBy(
            const float duration,
            const cocos2d::Size &size
        )
        {
            return fluxion::ResizeBy(duration, size);
        }

        inline fluxion::ResizeBy resizeBy(
            const float duration,
            const float width, const float height
        )
        {
            return fluxion::ResizeBy(duration, width, height);
        }
    }
}

#endif

