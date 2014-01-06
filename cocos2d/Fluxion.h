#ifndef __OREORE_COCOS2D_FLUXION_H__
#define __OREORE_COCOS2D_FLUXION_H__

/*
 * Fluxion -FLUently aCTION Generator-
*/

#include "cocos2d.h"
#include "../Utils.h"

namespace oreore
{
    namespace Fluxion
    {
        class Action
        {
        private:
            float time;
        public:
            Action() { }
            virtual ~Action() { }

            Action &operator&&const Action &action);
            virtual operator cocos2d::Action *();

            inline void setDuration(const float t) { time = t; }
            inline float getDuration() const { return time; }
        };

        class SequentialAction : public Action
        {
        private:
        public:
            SequentialAction() { }
            ~SequentialAction() { }

            operator cocos2d::Action *();
        };

        class ParallelAction : public Action
        {
        private:
        public:
            ParallelAction() { }
            ~ParallelAction() { }

            operator cocos2d::Action *();
            ParallelAction &operator|(const Action &action);
            inline ParallelAction &operator||(const Action &action) { return operator|(action); }
        };
    }

    // global operator
    template<typename T>
    inline T &operator |(
        const float time,
        const T &action
    ) {
        action.setDuration(time);
    }

    namespace x
    {
        class MoveTo : public Fluxion::Action
        {
        private:
        public:
            MoveTo() { }
            ~MoveTo() { }

            operator cocos2d::Action *();
        };
    }
}

#endif
