#ifndef __OREORE_COCOS2D_FLUXION_H__
#define __OREORE_COCOS2D_FLUXION_H__

/*
 * Fluxion -FLUently aCTION Generator-
 */

#include "cocos2d.h"

namespace oreore
{
    namespace fluxion
    {
        class SequentialAction;
        class ParallelAction;
        class Repeat;
        class ActionModifier;

        using ActionList = cocos2d::Vector<cocos2d::FiniteTimeAction *>;

        class ActionBase
        {
        public:
            ActionBase() { }
            virtual ~ActionBase() { }
            inline SequentialAction operator>>(const ActionBase &action);
            inline SequentialAction operator>>(cocos2d::FiniteTimeAction *action);
            inline SequentialAction operator>>(const std::function<void()> &action);
            inline ParallelAction operator+(const ActionBase &action);
            inline ParallelAction operator+(cocos2d::FiniteTimeAction *action);
            inline Repeat operator*(const int count);
            virtual operator cocos2d::FiniteTimeAction *() const = 0;
        };

        class ActionIntervalBase : public ActionBase
        {
        protected:
            template<typename T>
            inline static void addAction(const std::unique_ptr<ActionList> &actions, const T &a)
            {
                cocos2d::FiniteTimeAction *action = static_cast<cocos2d::FiniteTimeAction *>(a);
                if(action)
                {
                    actions->pushBack(action);
                }
            }
        public:
            ActionIntervalBase() { }
            virtual ~ActionIntervalBase() { }

            virtual operator cocos2d::ActionInterval *() const = 0;

            template<
                typename T,
                typename = typename std::enable_if<std::is_base_of<ActionModifier, T>::value>::type
            >
            inline T operator*(const T &op) { return op.apply(this); }

            template<
                typename T,
                typename = typename std::enable_if<std::numeric_limits<T>::is_integer>::type
            >
            inline Repeat operator*(const T &op);
        };

        template<typename T>
        class Action : public ActionBase
        {
        private:
            T *action;
        public:
            template<typename... Args>
            inline Action(const Args &...args) : action(T::create(args...)) { }
            inline operator cocos2d::FiniteTimeAction *() const override { return action; }
        };

        template<typename T>
        class ActionInterval : public ActionIntervalBase
        {
        private:
            T *action;
        public:
            template<typename... Args>
            inline ActionInterval(const Args &...args) : action(T::create(args...)) { }
            inline operator cocos2d::FiniteTimeAction *() const override { return action; }
            inline operator cocos2d::ActionInterval *() const override { return action; }
        };

        class EmptyAction : public ActionBase
        {
        private:
        public:
            EmptyAction() { }
            ~EmptyAction() { }
            inline operator cocos2d::FiniteTimeAction *() const override { return nullptr; }
        };

        class SequentialAction : public ActionIntervalBase
        {
        private:
            std::unique_ptr<ActionList> actions;
        public:
            inline SequentialAction(SequentialAction && rhs) : actions(std::move(rhs.actions)) { }
            inline SequentialAction() : actions(new ActionList())
            {
                actions->reserve(4);
            }

            template<typename S, typename T>
            inline SequentialAction(const S &a, const T &b) : actions(new ActionList())
            {
                addAction(actions, a);
                addAction(actions, b);
            }

            inline SequentialAction(ActionList *array) : actions(array) { }

            inline SequentialAction &operator>>(const ActionBase &action)
            {
                addAction(actions, action);
                return *this;
            }

            inline SequentialAction &operator>>(cocos2d::FiniteTimeAction *action)
            {
                if(action)
                {
                    actions->pushBack(action);
                }

                return *this;
            }

            inline SequentialAction &operator>>(const std::function<void()> &func)
            {
                actions->pushBack(cocos2d::CallFunc::create(func));
                return *this;
            }

            inline operator cocos2d::FiniteTimeAction *() const override
            {
                return cocos2d::Sequence::create(*actions.get());
            }

            inline operator cocos2d::ActionInterval *() const override
            {
                return cocos2d::Sequence::create(*actions.get());
            }

            template<typename... Args>
            inline static SequentialAction createFromActions(const Args &...args)
            {
                return SequentialAction(cocos2d::Array::create(static_cast<cocos2d::FiniteTimeAction *>(args)..., nullptr));
            }
        };

        class ParallelAction : public ActionIntervalBase
        {
        private:
            std::unique_ptr<ActionList> actions;
        public:
            inline ParallelAction(ParallelAction && rhs) : actions(std::move(rhs.actions)) { }
            inline ParallelAction() : actions(new ActionList())
            {
                actions->reserve(2);
            }

            inline ParallelAction(ActionList *array) : actions(array) { }

            template<typename S, typename T>
            inline ParallelAction(const S &a, const T &b) : actions(new ActionList())
            {
                addAction(actions, a);
                addAction(actions, b);
            }

            inline ParallelAction &operator+(const ActionBase &action)
            {
                addAction(actions, action);
                return *this;
            }

            inline ParallelAction &operator+(cocos2d::FiniteTimeAction *action)
            {
                if(action)
                {
                    actions->pushBack(action);
                }

                return *this;
            }

            inline operator cocos2d::FiniteTimeAction *() const override
            {
                return cocos2d::Spawn::create(*actions.get());
            }

            inline operator cocos2d::ActionInterval *() const override
            {
                return cocos2d::Spawn::create(*actions.get());
            }

            template<typename... Args>
            inline static ParallelAction createFromActions(const Args &...args)
            {
                return ParallelAction(cocos2d::Array::create(static_cast<cocos2d::FiniteTimeAction *>(args)..., nullptr));
            }
        };

        class Repeat : public ActionIntervalBase
        {
            cocos2d::Repeat *action;
        private:
        public:
            Repeat(const ActionBase &action, const unsigned int times) : action(cocos2d::Repeat::create(action, times)) { }
            Repeat(cocos2d::ActionInterval *action, const unsigned int times) : action(cocos2d::Repeat::create(action, times)) { }
            inline operator cocos2d::FiniteTimeAction *() const override { return action; }
            inline operator cocos2d::ActionInterval *() const override { return action; }
        };

        class ActionModifier : public ActionIntervalBase
        {
        private:
            cocos2d::ActionInterval *action;
        protected:
            inline cocos2d::ActionInterval *get() const { return action; }
        public:
            ActionModifier() : action(nullptr) { }
            ActionModifier(cocos2d::ActionInterval *action) : action(action) { }
            virtual ~ActionModifier() { }

            inline operator cocos2d::ActionInterval *() const override { return get(); }
        };

        template<typename T>
        class Ease : public ActionModifier
        {
        public:
            Ease() : ActionModifier(nullptr) { }
            Ease(T *ease) : ActionModifier(ease) { }
            ~Ease() { }

            inline operator cocos2d::FiniteTimeAction *() const { return get(); }
            inline Ease<T> apply(const ActionIntervalBase *action) const
            {
                return Ease<T>(T::create(static_cast<cocos2d::ActionInterval *>(*action)));
            }
        };

        template<typename T, typename P>
        class Ease1 : public ActionModifier
        {
        private:
            P param;
        public:
            Ease1() : ActionModifier(nullptr), param() { }
            Ease1(T *ease) : ActionModifier(ease), param() { }
            Ease1(const P param) : ActionModifier(nullptr), param(param) { }
            Ease1(T *ease, const P param) : ActionModifier(ease), param(param) { }
            ~Ease1() { }

            inline operator cocos2d::FiniteTimeAction *() const { return get(); }
            inline Ease1<T, P> apply(const ActionIntervalBase *action) const
            {
                return Ease1<T, P>(T::create(static_cast<cocos2d::ActionInterval *>(*action), param), param);
            }
        };

        class RepeatForever : public ActionModifier
        {
        public:
            RepeatForever() : ActionModifier(nullptr) { }
            RepeatForever(cocos2d::Repeat *action) : ActionModifier(action) { }
            ~RepeatForever() { }

            inline operator cocos2d::FiniteTimeAction *() const { return get(); }
            inline RepeatForever apply(const ActionIntervalBase *action) const
            {
                return RepeatForever(cocos2d::Repeat::create(static_cast<cocos2d::ActionInterval *>(*action), -1));
            }
        };

        /* Action impl */
        inline SequentialAction ActionBase::operator>>(const ActionBase &action)
        {
            return SequentialAction(*this, action);
        }

        inline SequentialAction ActionBase::operator>>(cocos2d::FiniteTimeAction *action)
        {
            return SequentialAction(*this, action);
        }

        inline SequentialAction ActionBase::operator>>(const std::function<void()> &action)
        {
            return SequentialAction(*this, cocos2d::CallFunc::create(action));
        }

        inline ParallelAction ActionBase::operator+(const ActionBase &action)
        {
            return ParallelAction(*this, action);
        }

        inline ParallelAction ActionBase::operator+(cocos2d::FiniteTimeAction *action)
        {
            return ParallelAction(*this, action);
        }

        inline Repeat ActionBase::operator *(const int count)
        {
            return Repeat(*this, count);
        }

        /* ActionInterval impl */
        template<typename T, typename>
        inline Repeat ActionIntervalBase::operator*(const T &op)
        {
            return Repeat(*this, op);
        }

        /* action defines */
        /* finite time action */
        using MoveTo = ActionInterval<cocos2d::MoveTo>;
        using MoveBy = ActionInterval<cocos2d::MoveBy>;

        using FadeOut = ActionInterval<cocos2d::FadeOut>;
        using FadeIn  = ActionInterval<cocos2d::FadeIn>;
        using FadeTo  = ActionInterval<cocos2d::FadeTo>;

        using ScaleTo = ActionInterval<cocos2d::ScaleTo>;
        using ScaleBy = ActionInterval<cocos2d::ScaleBy>;

        using Blink = ActionInterval<cocos2d::Blink>;
        using Wait  = ActionInterval<cocos2d::DelayTime>;

        using RotateTo = ActionInterval<cocos2d::RotateTo>;
        using RotateBy = ActionInterval<cocos2d::RotateBy>;

        /* action */
        using Show = Action<cocos2d::Show>;
        using Hide = Action<cocos2d::Hide>;

        using CallFunc = Action<cocos2d::CallFunc>;

        /* ease */
        namespace ease
        {
            /* ease */
            using EaseIn    = fluxion::Ease1<cocos2d::EaseIn, float>;
            using EaseOut   = fluxion::Ease1<cocos2d::EaseOut, float>;
            using EaseInOut = fluxion::Ease1<cocos2d::EaseInOut, float>;

            /* bounce */
            using BounceIn    = fluxion::Ease<cocos2d::EaseBounceIn>;
            using BounceOut   = fluxion::Ease<cocos2d::EaseBounceOut>;
            using BounceInOut = fluxion::Ease<cocos2d::EaseBounceInOut>;

            /* back */
            using BackIn    = fluxion::Ease<cocos2d::EaseBackIn>;
            using BackOut   = fluxion::Ease<cocos2d::EaseBackOut>;
            using BackInOut = fluxion::Ease<cocos2d::EaseBackInOut>;

            /* exp */
            using ExpIn    = fluxion::Ease<cocos2d::EaseExponentialIn>;
            using ExpOut   = fluxion::Ease<cocos2d::EaseExponentialOut>;
            using ExpInOut = fluxion::Ease<cocos2d::EaseExponentialInOut>;

            /* sine */
            using SineIn    = fluxion::Ease<cocos2d::EaseSineIn>;
            using SineOut   = fluxion::Ease<cocos2d::EaseSineOut>;
            using SineInOut = fluxion::Ease<cocos2d::EaseSineInOut>;

            /* elastic */
            using ElasticIn    = fluxion::Ease1<cocos2d::EaseElasticIn, float>;
            using ElasticOut   = fluxion::Ease1<cocos2d::EaseElasticOut, float>;
            using ElasticInOut = fluxion::Ease1<cocos2d::EaseElasticInOut, float>;
        }

        inline SequentialAction operator>>(cocos2d::Action *a, const ActionBase &b)
        {
            return SequentialAction(a, b);
        }

        inline SequentialAction operator>>(const std::function<void()> &func, const ActionBase &b)
        {
            return SequentialAction(cocos2d::CallFunc::create(func), b);
        }

        inline ParallelAction operator+(cocos2d::FiniteTimeAction *a, const ActionBase &b)
        {
            return ParallelAction(a, static_cast<cocos2d::FiniteTimeAction *>(b));
        }
    }

    namespace x
    {
        /* finite time action */
        inline fluxion::MoveTo moveTo(const float duration, const cocos2d::Point &pos) { return fluxion::MoveTo(duration, pos); }
        inline fluxion::MoveTo moveTo(const float duration, const float x, const float y) { return moveTo(duration, cocos2d::Point(x, y)); }
        inline fluxion::MoveBy moveBy(const float duration, const cocos2d::Point &delta) { return fluxion::MoveBy(duration, delta); }
        inline fluxion::MoveBy moveBy(const float duration, const float dx, const float dy) { return moveBy(duration, cocos2d::Point(dx, dy)); }

        inline fluxion::FadeOut fadeOut(const float duration) { return fluxion::FadeOut(duration); }
        inline fluxion::FadeIn fadeIn(const float duration) { return fluxion::FadeIn(duration); }
        inline fluxion::FadeTo fadeTo(const float duration, const GLubyte opacity) { return fluxion::FadeTo(duration, opacity); }

        inline fluxion::ScaleTo scaleTo(const float duration, const float s) { return fluxion::ScaleTo(duration, s); }
        inline fluxion::ScaleTo scaleTo(const float duration, const float sx, const float sy) { return fluxion::ScaleTo(duration, sx, sy); }
        inline fluxion::ScaleBy scaleBy(const float duration, const float s) { return fluxion::ScaleBy(duration, s); }
        inline fluxion::ScaleBy scaleBy(const float duration, const float sx, const float sy) { return fluxion::ScaleBy(duration, sx, sy); }

        inline fluxion::Blink blink(const float duration, const int blinks) { return fluxion::Blink(duration, blinks); }

        inline fluxion::Wait wait(const float duration) { return fluxion::Wait(duration); }
        inline fluxion::Wait delayTime(const float duration) { return wait(duration); }

        inline fluxion::RotateTo rotateTo(const float duration, const float angle) { return fluxion::RotateTo(duration, angle); }
        inline fluxion::RotateTo rotateTo(const float duration, const float angleX, const float angleY) { return fluxion::RotateTo(duration, angleX, angleY); }

        inline fluxion::RotateBy rotateBy(const float duration, const float angle) { return fluxion::RotateBy(duration, angle); }
        inline fluxion::RotateBy rotateBy(const float duration, const float angleX, const float angleY) { return fluxion::RotateBy(duration, angleX, angleY); }

        /* action */
        inline fluxion::Show show() { return fluxion::Show(); }
        inline fluxion::Hide hide() { return fluxion::Hide(); }

        inline fluxion::CallFunc call(const std::function<void()> &func) { return fluxion::CallFunc(func); }

        template<typename... Args>
        inline fluxion::ParallelAction p(const Args &...args) { return fluxion::ParallelAction::createFromActions(args...); }
        template<typename... Args>
        inline fluxion::ParallelAction par(const Args &...args) { return p(args...); }

        template<typename... Args>
        inline fluxion::SequentialAction s(const Args &...args) { return fluxion::SequentialAction::createFromActions(args...); }
        template<typename... Args>
        inline fluxion::SequentialAction seq(const Args &...args) { return s(args...); }

        /* repeat */
        inline fluxion::RepeatForever infinite() { return fluxion::RepeatForever(); }
        inline fluxion::RepeatForever inf() { return fluxion::RepeatForever(); }

        /* noop */
        inline fluxion::EmptyAction noop() { return fluxion::EmptyAction(); }

        /* action convert */
        template<
            typename T, typename... Args,
            typename = typename std::enable_if<std::is_base_of<cocos2d::ActionInterval, T>::value, void>::type
        >
        inline fluxion::ActionInterval<T> flux(const Args &...args) { return fluxion::ActionInterval<T>(args...); }

        template<
            typename T, typename... Args,
            typename = typename std::enable_if<
                !std::is_base_of<cocos2d::ActionInterval, T>::value &&
                std::is_base_of<cocos2d::Action, T>::value,
                void
            >::type
        >
        inline fluxion::Action<T> flux(const Args &...args) { return fluxion::Action<T>(args...); }

        namespace ease
        {
            /* ease */
            inline fluxion::ease::EaseIn easeIn(const float rate) { return fluxion::ease::EaseIn(rate); }
            inline fluxion::ease::EaseOut easeOut(const float rate) { return fluxion::ease::EaseOut(rate); }
            inline fluxion::ease::EaseInOut easeInOut(const float rate) { return fluxion::ease::EaseInOut(rate); }

            /* bounce */
            inline fluxion::ease::BounceIn bounceIn() { return fluxion::ease::BounceIn(); }
            inline fluxion::ease::BounceOut bounceOut() { return fluxion::ease::BounceOut(); }
            inline fluxion::ease::BounceInOut bounceInOut() { return fluxion::ease::BounceInOut(); }

            /* back */
            inline fluxion::ease::BackIn backIn() { return fluxion::ease::BackIn(); }
            inline fluxion::ease::BackOut backOut() { return fluxion::ease::BackOut(); }
            inline fluxion::ease::BackInOut backInOut() { return fluxion::ease::BackInOut(); }

            /* exp */
            inline fluxion::ease::ExpIn expIn() { return fluxion::ease::ExpIn(); }
            inline fluxion::ease::ExpOut expOut() { return fluxion::ease::ExpOut(); }
            inline fluxion::ease::ExpInOut expInOut() { return fluxion::ease::ExpInOut(); }

            /* sign */
            inline fluxion::ease::SineIn sineIn() { return fluxion::ease::SineIn(); }
            inline fluxion::ease::SineOut sineOut() { return fluxion::ease::SineOut(); }
            inline fluxion::ease::SineInOut sineInOut() { return fluxion::ease::SineInOut(); }

            /* ease */
            inline fluxion::ease::ElasticIn elasticIn(const float period) { return fluxion::ease::ElasticIn(period); }
            inline fluxion::ease::ElasticOut elasticOut(const float period) { return fluxion::ease::ElasticOut(period); }
            inline fluxion::ease::ElasticInOut elasticInOut(const float period) { return fluxion::ease::ElasticInOut(period); }
        }

        // for old oreore compatibility
        namespace Ease = ease;
    }
}

namespace cocos2d
{
    inline oreore::fluxion::SequentialAction operator>>(Action *a, const std::function<void()> &func)
    {
        return oreore::fluxion::SequentialAction(a, CallFunc::create(func));
    }
}

#endif

