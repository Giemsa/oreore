#ifndef __OREORE_COCOS2D_CALLINTERVAL_H__
#define __OREORE_COCOS2D_CALLINTERVAL_H__

#include "cocos2d.h"

namespace oreore
{
    class CallInterval : public cocos2d::ActionInterval
    {
    private:
        std::function<void (float)> func;
    public:
        CallInterval() { }
        virtual ~CallInterval() { }

        static CallInterval* create(const float duration, const std::function<void (float)> &func);
        virtual bool initWithDuration(const float duration, const std::function<void (float)> &func);

        virtual void update(float time) override;

        virtual CallInterval *reverse() const override;
        virtual CallInterval *clone() const override;
    };
}

#endif

