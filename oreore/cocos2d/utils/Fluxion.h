#ifndef __OREORE_COCOS2D_FLUXION_H__
#define __OREORE_COCOS2D_FLUXION_H__

/*
 * Fluxion -FLUently aCTION Generator-
 */

#include "cocos2d.h"

namespace oreore
{
    namespace Fluxion
    {
        class SequentialAction;

        struct Action
        {
            inline SequentialAction operator>>(const Action &action);
            virtual operator cocos2d::Action *() const = 0;
        };

        template<typename T>
        class WrapAction : public Action
        {
        private:
            T *action;
        public:
            template<typename... Args>
            inline WrapAction(const Args... args) : action(T::create(args...)) { }
            inline operator cocos2d::Action *() const override { return action; }
        };

        template<typename T>
        class WrapFiniteAction : public Action
        {
        private:
            T *action;
        public:
            template<typename... Args>
            inline WrapFiniteAction(const Args... args) : action(T::create(args...)) { }
            inline operator cocos2d::FiniteTimeAction *() const { return action; }
            inline operator cocos2d::Action *() const override { return action; }
        };

        class SequentialAction : public Action
        {
        private:
            std::unique_ptr<cocos2d::Array> actions;
        public:
            inline SequentialAction() : actions(new cocos2d::Array())
            {
                actions->initWithCapacity(4);
            }

            inline SequentialAction(const Action &a, const Action &b) : actions(new cocos2d::Array())
            {
                actions->initWithObject(static_cast<cocos2d::Action *>(a));
                actions->addObject(static_cast<cocos2d::Action *>(b));
            }

            inline SequentialAction &operator>>(const Action &action)
            {
                actions->addObject(static_cast<cocos2d::Action *>(action));
                return *this;
            }

            inline SequentialAction &operator>>(cocos2d::Action *action)
            {
                actions->addObject(action);
                return *this;
            }

            inline operator cocos2d::Action *() const override
            {
                return cocos2d::Sequence::create(actions.get());
            }
        };

        /* Action impl */
        inline SequentialAction Action::operator>>(const Action &action)
        {
            return SequentialAction(*this, action);
        }

        class ParallelAction : public Action
        {
        private:
            cocos2d::Spawn *actions;
        public:
            template<typename... Args>
            ParallelAction(const Args... args) : actions(cocos2d::Spawn::create(static_cast<cocos2d::FiniteTimeAction *>(args)..., nullptr)) { }
            ~ParallelAction() { }

            inline operator cocos2d::Action *() const override { return actions; }
        };
    }

    namespace x
    {
        class MoveTo : public Fluxion::Action
        {
        private:
            cocos2d::MoveTo *action;
        public:
            inline MoveTo(const float duration, const float x, const float y)
                : action(cocos2d::MoveTo::create(duration, cocos2d::Point(x, y))) { }
            inline MoveTo(const float duration, const cocos2d::Point &pos)
                : action(cocos2d::MoveTo::create(duration, pos)) { }
            inline operator cocos2d::Action *() const override { return action; }
            inline operator cocos2d::FiniteTimeAction *() const { return action; }
        };

        typedef Fluxion::WrapFiniteAction<cocos2d::FadeOut> FadeOut;

        inline MoveTo moveTo(const float duration, const float x, const float y) { return MoveTo(duration, x, y); }
        inline MoveTo moveTo(const float duration, const cocos2d::Point &pos) { return moveTo(duration, pos.x, pos.y); }
        inline FadeOut fadeOut(const float duration) { return FadeOut(duration); }

        template<typename... Args>
        inline Fluxion::ParallelAction p(const Args... args) { return Fluxion::ParallelAction(args...); }
    }
}

#endif
