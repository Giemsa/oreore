#ifndef __OREORE_COCOS2D_MOVE_H__
#define __OREORE_COCOS2D_MOVE_H__

#include "cocos2d.h"

namespace oreore
{
    class Move : public cocos2d::ActionInterval
    {
    private:
        cocos2d::Point vec;
    public:
        Move() { }
        virtual ~Move() { }

        static Move* create(const float duration, const cocos2d::Point &vec);
        bool initWithDuration(const float duration, const cocos2d::Point &vec);

        virtual void startWithTarget(cocos2d::Node *pTarget);
        virtual void update(float time);
        virtual void stop(void) override;

        virtual Move *reverse() const override;
        virtual Move *clone() const override;
    };
}

#endif

