#ifndef __OREORE_COCOS2D_FLUXION_H__
#define __OREORE_COCOS2D_FLUXION_H__

/*
 * Fluxion -FLUently aCTION Generator-
 */

#include <limits>
#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    namespace Fluxion
    {
        template<class Base, class Sub>
        class is_base_of
        {
            typedef int T;
            struct F { int i, j; };
        private:
            static Sub *d;
            static T check(const Base &);
            static F check(...);
        public:
            static const bool value = sizeof(check(*d)) == sizeof(T);
        };

        template<bool B>
        struct enable_if { typedef int type; };

        template<>
        struct enable_if<false> { };

        class SequentialAction;
        class ParallelAction;
        class Repeat;
        class Ease;

        template<typename T, typename S>
        struct ResultType { typedef T type; };

        class Action
        {
        public:
            Action() { }
            virtual ~Action() { }
            inline SequentialAction operator>>(const Action &action);
            inline SequentialAction operator>>(cocos2d::CCFiniteTimeAction *action);
            inline ParallelAction operator+(const Action &action);
            inline ParallelAction operator+(cocos2d::CCFiniteTimeAction *action);
            inline Repeat operator*(const int count);
            virtual operator cocos2d::CCFiniteTimeAction *() const = 0;
        };

        class ActionInterval : public Action
        {
        public:
            ActionInterval() { }
            virtual ~ActionInterval() { }
            virtual operator cocos2d::CCActionInterval *() const = 0;

            template<typename T>
            inline typename ResultType<T, typename enable_if<is_base_of<Ease, T>::value>::type>::type operator*(const T &op)
            {
                return op.apply(this);
            }

            template<typename T>
            inline typename ResultType<Repeat, typename enable_if<std::numeric_limits<T>::is_integer>::type>::type operator*(const T &op);
        };

        template<typename T>
        class WrapAction : public Action
        {
        private:
            T *action;
        public:
            inline WrapAction(T *action) : action(action) { }
            inline operator cocos2d::CCFiniteTimeAction *() const { return action; } // override
        };

        template<typename T>
        class WrapFiniteAction : public ActionInterval
        {
        private:
            T *action;
        public:
            inline WrapFiniteAction(T *action) : action(action) { }
            inline operator cocos2d::CCActionInterval *() const { return action; } // override
            inline operator cocos2d::CCFiniteTimeAction *() const { return action; } // override
        };

        class SequentialAction : public ActionInterval
        {
        private:
            std::auto_ptr<cocos2d::CCArray> actions;
        public:
            inline SequentialAction(SequentialAction && rhs) : actions(rhs.actions) { }
            inline SequentialAction() : actions(new cocos2d::CCArray())
            {
                actions->initWithCapacity(4);
            }

            template<typename S, typename T>
            inline SequentialAction(const S &a, const T &b) : actions(new cocos2d::CCArray())
            {
                actions->initWithObject(static_cast<cocos2d::CCFiniteTimeAction *>(a));
                actions->addObject(static_cast<cocos2d::CCFiniteTimeAction *>(b));
            }

            inline SequentialAction(cocos2d::CCArray *array) : actions(array) { }

            inline SequentialAction &operator>>(const Action &action)
            {
                actions->addObject(static_cast<cocos2d::CCAction *>(action));
                return *this;
            }

            inline SequentialAction &operator>>(cocos2d::CCAction *action)
            {
                actions->addObject(action);
                return *this;
            }

            inline operator cocos2d::CCFiniteTimeAction *() const // override
            {
                return cocos2d::CCSequence::create(actions.get());
            }

            inline operator cocos2d::CCActionInterval *() const // override
            {
                return cocos2d::CCSequence::create(actions.get());
            }
        };

        class ParallelAction : public ActionInterval
        {
            friend class ActionInterval;
        private:
            std::auto_ptr<cocos2d::CCArray> actions;
        public:
            inline ParallelAction(ParallelAction && rhs) : actions(rhs.actions) { }
            inline ParallelAction() : actions(new cocos2d::CCArray())
            {
                actions->initWithCapacity(2);
            }
            
            inline ParallelAction(cocos2d::CCArray *array) : actions(array) { }

            template<typename S, typename T>
            inline ParallelAction(const S &a, const T &b) : actions(new cocos2d::CCArray())
            {
                actions->initWithObject(static_cast<cocos2d::CCFiniteTimeAction *>(a));
                actions->addObject(static_cast<cocos2d::CCFiniteTimeAction *>(b));
            }

            inline ParallelAction &operator+(const ActionInterval &action)
            {
                actions->addObject(static_cast<cocos2d::CCActionInterval *>(action));
                return *this;
            }

            inline ParallelAction &operator+(cocos2d::CCActionInterval *action)
            {
                actions->addObject(action);
                return *this;
            }

            inline operator cocos2d::CCFiniteTimeAction *() const // override
            {
                return cocos2d::CCSpawn::create(actions.get());
            }

            inline operator cocos2d::CCActionInterval *() const // override
            {
                return cocos2d::CCSpawn::create(actions.get());
            }
        };

        class Repeat : public ActionInterval
        {
            cocos2d::CCRepeat *action;
        private:
        public:
            Repeat(const ActionInterval &action, const int times) : action(cocos2d::CCRepeat::create(action, times)) { }
            inline operator cocos2d::CCFiniteTimeAction *() const { return action; } // override
            inline operator cocos2d::CCActionInterval *() const { return action; } // override
        };

        class Ease : public ActionInterval
        {
        private:
            cocos2d::CCActionInterval *ease;
        protected:
            inline cocos2d::CCActionInterval *get() const { return ease; }
        public:
            Ease() : ease(null) { }
            Ease(cocos2d::CCActionInterval *ease) : ease(ease) { }
            virtual ~Ease() { }

            inline operator cocos2d::CCActionInterval *() const{ return get(); } // override
        };

        template<typename T>
        class WrapEase : public Ease
        {
        private:
            T *ease;
        public:
            WrapEase() : ease(null) { }
            WrapEase(T *ease) : ease(ease) { }
            ~WrapEase() { }

            inline operator cocos2d::CCFiniteTimeAction *() const { return ease; }
            inline operator cocos2d::CCActionInterval *() const { return ease; }
            inline WrapEase<T> apply(const ActionInterval *action) const
            {
                return WrapEase<T>(T::create(static_cast<cocos2d::CCActionInterval *>(*action)));
            }
        };

        template<typename T, typename P>
        class WrapEase1 : public Ease
        {
        private:
            T *ease;
            P param;
        public:
            WrapEase1() : ease(null), param() { }
            WrapEase1(T *ease) : ease(ease), param() { }
            WrapEase1(const P param) : ease(ease), param(param) { }
            WrapEase1(T *ease, const P param) : ease(ease), param(param) { }
            ~WrapEase1() { }

            inline operator cocos2d::CCFiniteTimeAction *() const { return ease; }
            inline operator cocos2d::CCActionInterval *() const { return ease; }
            inline WrapEase1<T, P> apply(const ActionInterval *action) const
            {
                return WrapEase1<T, P>(T::create(static_cast<cocos2d::CCActionInterval *>(*action), param), param);
            }
        };

        /* Action impl */
        inline SequentialAction Action::operator>>(const Action &action)
        {
            return SequentialAction(*this, action);
        }
    
        inline SequentialAction Action::operator>>(cocos2d::CCFiniteTimeAction *action)
        {
            return SequentialAction(*this, action);
        }

        /* Action impl */
        inline ParallelAction Action::operator+(const Action &action)
        {
            return ParallelAction(*this, action);
        }
    
        inline ParallelAction Action::operator+(cocos2d::CCFiniteTimeAction *action)
        {
            return ParallelAction(*this, action);
        }

        template<typename T>
        inline typename ResultType<Repeat, typename enable_if<std::numeric_limits<T>::is_integer>::type>::type ActionInterval::operator*(const T &op)
        {
            return Repeat(*this, op);
        }
    }

    inline Fluxion::SequentialAction operator>>(cocos2d::CCAction *a, const Fluxion::Action &b)
    {
        return Fluxion::SequentialAction(a, b);
    }

    inline Fluxion::ParallelAction operator+(cocos2d::CCFiniteTimeAction *a, const Fluxion::ActionInterval &b)
    {
        return Fluxion::ParallelAction(a, static_cast<cocos2d::CCFiniteTimeAction *>(b));
    }

    namespace x
    {
        /* finite time action */
        typedef Fluxion::WrapFiniteAction<cocos2d::CCMoveTo> MoveTo;
        inline MoveTo moveTo(const float duration, const cocos2d::CCPoint &pos) { return MoveTo(cocos2d::CCMoveTo::create(duration, pos)); }
        inline MoveTo moveTo(const float duration, const float x, const float y) { return moveTo(duration, cocos2d::CCPoint(x, y)); }
        
        typedef Fluxion::WrapFiniteAction<cocos2d::CCMoveBy> MoveBy;
        inline MoveBy moveBy(const float duration, const cocos2d::CCPoint &delta) { return cocos2d::CCMoveBy::create(duration, delta); }
        inline MoveBy moveBy(const float duration, const float dx, const float dy) { return moveBy(duration, cocos2d::CCPoint(dx, dy)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCFadeOut> FadeOut;
        inline FadeOut fadeOut(const float duration) { return FadeOut(cocos2d::CCFadeOut::create(duration)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCFadeIn> FadeIn;
        inline FadeIn fadeIn(const float duration) { return FadeIn(cocos2d::CCFadeIn::create(duration)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCFadeTo> FadeTo;
        inline FadeTo fadeTo(const float duration, const GLubyte opacity) { return FadeTo(cocos2d::CCFadeTo::create(duration, opacity)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCScaleTo> ScaleTo;
        inline ScaleTo scaleTo(const float duration, const float s) { return ScaleTo(cocos2d::CCScaleTo::create(duration, s)); }
        inline ScaleTo scaleTo(const float duration, const float sx, const float sy) { return ScaleTo(cocos2d::CCScaleTo::create(duration, sx, sy)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCScaleBy> ScaleBy;
        inline ScaleBy scaleBy(const float duration, const float s) { return ScaleBy(cocos2d::CCScaleBy::create(duration, s)); }
        inline ScaleBy scaleBy(const float duration, const float sx, const float sy) { return ScaleBy(cocos2d::CCScaleBy::create(duration, sx, sy)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCBlink> Blink;
        inline Blink blink(const float duration, const int blinks) { return Blink(cocos2d::CCBlink::create(duration, blinks)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCDelayTime> Wait;
        inline Wait wait(const float duration) { return Wait(cocos2d::CCDelayTime::create(duration)); }
        inline Wait delayTime(const float duration) { return Wait(cocos2d::CCDelayTime::create(duration)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCRotateTo> RotateTo;
        inline RotateTo rotateTo(const float duration, const float angle) { return RotateTo(cocos2d::CCRotateTo::create(duration, angle)); }
        inline RotateTo rotateTo(const float duration, const float angleX, const float angleY) { return RotateTo(cocos2d::CCRotateTo::create(duration, angleX, angleY)); }

        typedef Fluxion::WrapFiniteAction<cocos2d::CCRotateBy> RotateBy;
        inline RotateBy rotateBy(const float duration, const float angle) { return RotateBy(cocos2d::CCRotateBy::create(duration, angle)); }
        inline RotateBy rotateBy(const float duration, const float angleX, const float angleY) { return RotateBy(cocos2d::CCRotateBy::create(duration, angleX, angleY)); }


        /* action */
        typedef Fluxion::WrapAction<cocos2d::CCShow> Show;
        inline Show show() { return Show(cocos2d::CCShow::create()); }

        typedef Fluxion::WrapAction<cocos2d::CCHide> Hide;
        inline Hide hide() { return Hide(cocos2d::CCHide::create()); }

        typedef Fluxion::WrapAction<cocos2d::CCCallFunc> CallFunc;
        template<typename T>
        inline CallFunc call(cocos2d::CCObject *target, void (T::*callback)()) { return CallFunc(cocos2d::CCCallFunc::create(target, static_cast<cocos2d::SEL_CallFunc>(callback))); }

#define TUPLE(ARG, ...) ARG, __VA_ARGS__
#define TT(N) typename T ## N
#define ARG(N) static_cast<cocos2d::CCActionInterval *>(arg ## N)
#define PRM(N) const T ## N arg ## N

#define PAR(T, P, A)  \
    template<T>     \
    inline Fluxion::ParallelAction p(P) { return Fluxion::ParallelAction(cocos2d::CCArray::create(A)); }    \
    template<T>     \
    inline Fluxion::ParallelAction par(P) { return Fluxion::ParallelAction(cocos2d::CCArray::create(A)); }

#define SEQ(T, P, A)  \
    template<T>     \
    inline Fluxion::SequentialAction s(P) { return Fluxion::SequentialAction(cocos2d::CCArray::create(A)); }    \
    template<T>     \
    inline Fluxion::SequentialAction seq(P) { return Fluxion::SequentialAction(cocos2d::CCArray::create(A)); }

#define DEF_FUNCS(F)    \
        F(TT(0), PRM(0), ARG(0))    \
        F(TUPLE(TT(0), TT(1)), TUPLE(PRM(0), PRM(1)), TUPLE(ARG(0), ARG(1)))    \
        F(TUPLE(TT(0), TT(1), TT(2)), TUPLE(PRM(0), PRM(1), PRM(2)), TUPLE(ARG(0), ARG(1), ARG(2)))    \
        F(TUPLE(TT(0), TT(1), TT(2), TT(3)), TUPLE(PRM(0), PRM(1), PRM(2), PRM(3)), TUPLE(ARG(0), ARG(1), ARG(2), ARG(3)))    \
        F(TUPLE(TT(0), TT(1), TT(2), TT(3), TT(4)), TUPLE(PRM(0), PRM(1), PRM(2), PRM(3), PRM(4)), TUPLE(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4)))    \
        F(TUPLE(TT(0), TT(1), TT(2), TT(3), TT(4), TT(5)), TUPLE(PRM(0), PRM(1), PRM(2), PRM(3), PRM(4), PRM(5)), TUPLE(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4), ARG(5)))    \
        F(TUPLE(TT(0), TT(1), TT(2), TT(3), TT(4), TT(5), TT(6)), TUPLE(PRM(0), PRM(1), PRM(2), PRM(3), PRM(4), PRM(5), PRM(6)), TUPLE(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4), ARG(5), ARG(6)))    \
        F(TUPLE(TT(0), TT(1), TT(2), TT(3), TT(4), TT(5), TT(6), TT(7)), TUPLE(PRM(0), PRM(1), PRM(2), PRM(3), PRM(4), PRM(5), PRM(6), PRM(7)), TUPLE(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4), ARG(5), ARG(6), ARG(7)))    \

        DEF_FUNCS(PAR)
        DEF_FUNCS(SEQ)

#undef DEF_FUNCS
#undef SEQ
#undef PAR
#undef PRM
#undef ARG
#undef TT
#undef TUPLE

        namespace Ease
        {
            /* ease */
            typedef Fluxion::WrapEase1<cocos2d::CCEaseIn, float> EaseIn;
            inline EaseIn easeIn(const float rate) { return EaseIn(rate); }

            typedef Fluxion::WrapEase1<cocos2d::CCEaseOut, float> EaseOut;
            inline EaseOut easeOut(const float rate) { return EaseOut(rate); }

            typedef Fluxion::WrapEase1<cocos2d::CCEaseInOut, float> EaseInOut;
            inline EaseInOut easeInOut(const float rate) { return EaseInOut(rate); }

            /* bounce */
            typedef Fluxion::WrapEase<cocos2d::CCEaseBounceIn> BounceIn;
            inline BounceIn bounceIn() { return BounceIn(); }

            typedef Fluxion::WrapEase<cocos2d::CCEaseBounceOut> BounceOut;
            inline BounceOut bounceOut() { return BounceOut(); }

            typedef Fluxion::WrapEase<cocos2d::CCEaseBounceInOut> BounceInOut;
            inline BounceInOut bounceInOut() { return BounceInOut(); }
            
            /* exp */
            typedef Fluxion::WrapEase<cocos2d::CCEaseExponentialIn> ExpIn;
            inline ExpIn expIn() { return ExpIn(); }

            typedef Fluxion::WrapEase<cocos2d::CCEaseExponentialOut> ExpOut;
            inline ExpOut expOut() { return ExpOut(); }

            typedef Fluxion::WrapEase<cocos2d::CCEaseExponentialInOut> ExpInOut;
            inline ExpInOut expInOut() { return ExpInOut(); }

            /* sign */
            typedef Fluxion::WrapEase<cocos2d::CCEaseSineIn> SineIn;
            inline SineIn sineIn() { return SineIn(); }

            typedef Fluxion::WrapEase<cocos2d::CCEaseSineOut> SineOut;
            inline SineOut sineOut() { return SineOut(); }

            typedef Fluxion::WrapEase<cocos2d::CCEaseSineInOut> SineInOut;
            inline SineInOut sineInOut() { return SineInOut(); }
        }
    }
}

#endif
