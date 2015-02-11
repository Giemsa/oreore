#ifndef __OREORE_COCOS2D_RESIZETO_H__
#define __OREORE_COCOS2D_RESIZETO_H__

#include "cocos2d.h"
#include "ResizeBy.h"
#include "../utils/Fluxion.h"

namespace oreore
{
    class ResizeTo : public ResizeBy
    {
    private:
    public:
        ResizeTo() { }
        virtual ~ResizeTo() { }

        static ResizeTo* create(const float duration, const cocos2d::Size &size);
        static ResizeTo* create(const float duration, const float width, const float height)
        {
            return create(duration, cocos2d::Size(width, height));
        }

        virtual bool initWithDuration(const float duration, const cocos2d::Size &size);
        virtual void startWithTarget(cocos2d::Node *pTarget) override;

        virtual ResizeTo *reverse() const override;
        virtual ResizeTo *clone() const override;
    };

    // fluxion plugin
    namespace fluxion
    {
        using ResizeTo = ActionInterval<oreore::ResizeTo>;
    }

    namespace x
    {
        inline fluxion::ResizeTo resizeTo(
            const float duration,
            const cocos2d::Size &size
        )
        {
            return fluxion::ResizeTo(duration, size);
        }

        inline fluxion::ResizeTo resizeTo(
            const float duration,
            const float width, const float height
        )
        {
            return fluxion::ResizeTo(duration, width, height);
        }
    }
}

#endif

