#ifndef __OREORE_TUTORIAL_SEQUENCE_H__
#define __OREORE_TUTORIAL_SEQUENCE_H__

#include <vector>
#include <functional>
#include "buffer.hpp"

namespace oreore
{
    namespace Tutorial
    {
        // forward
        namespace detail
        {
            class TutorialPlayInfoBase; // in TutorialBase.h
        }

        class TutorialBase; // in TutorialBase.h

        class TutorialPhase final
        {
            friend class TutorialSequence;

            struct FunctorBase
            {
                virtual ~FunctorBase() { }
                virtual bool call(const size_t index) const = 0;
                virtual bool call(void *self, const size_t index) const = 0;
                virtual FunctorBase *clone() const = 0;
            };

            class Functor final : public FunctorBase
            {
            private:
                bool (* func)(const size_t);
            public:
                Functor()
                : func(nullptr)
                { }

                Functor(bool (* const func)(const size_t))
                : func(func)
                { }

                bool call(const size_t index) const override
                {
                    return func ? func(index) : true;
                }

                bool call(void *self, const size_t index) const override
                {
                    return call(index);
                }

                FunctorBase *clone() const override
                {
                    return new Functor(func);
                }
            };

            template<typename T>
            class MemFunctor final : public FunctorBase
            {
            private:
                bool (T::* func)(const size_t);
            public:
                MemFunctor()
                : func(nullptr)
                { }

                MemFunctor(bool (T::* const func)(const size_t))
                : func(func)
                { }

                bool call(const size_t index) const override
                {
                    return true;
                }

                bool call(void *self, const size_t index) const override
                {
                    return func ? (static_cast<T *>(self)->*func)(index) : true;
                }

                FunctorBase *clone() const override
                {
                    return new MemFunctor(func);
                }
            };
        private:
            FunctorBase *functor;
            TutorialPhase *next;
            size_t index;

            TutorialPhase(const TutorialPhase &rhs)
            : functor(rhs.functor->clone()), next(rhs.next), index(rhs.index)
            { }
        public:
            TutorialPhase()
            : functor(nullptr), next(nullptr), index(0)
            { }

            TutorialPhase(FunctorBase *functor)
            : functor(functor), next(nullptr), index(0)
            { }

            TutorialPhase(bool (* const func)(const size_t))
            : functor(new Functor(func)), next(nullptr), index(0)
            { }

            template<typename T>
            TutorialPhase(bool (T::* const func)(const size_t))
            : functor(new MemFunctor<T>(func)), next(nullptr), index(0)
            { }

            TutorialPhase(TutorialPhase &&rhs)
            : functor(rhs.functor), next(rhs.next), index(rhs.index)
            {
                rhs.functor = nullptr;
                rhs.next = nullptr;
                rhs.index = 0;
            }

            ~TutorialPhase()
            {
                delete functor;
            }

            bool proceed() const
            {
                return functor ? functor->call(index) : true;
            }

            template<typename T>
            bool proceed(T *self) const
            {
                return functor ? functor->call(self, index) : true;
            }

            TutorialPhase &operator=(const TutorialPhase &) = delete;
            TutorialPhase &operator=(TutorialPhase &&rhs)
            {
                delete functor;
                functor = rhs.functor;
                rhs.functor = nullptr;

                next = rhs.next;
                rhs.next = nullptr;

                index = rhs.index;
                rhs.index = 0;

                return *this;
            }
        };

        class TutorialSequence
        {
            template<typename T>
            friend class TutorialManager;

            using PhaseList = utils::buffer<TutorialPhase>;
        private:
            PhaseList phaseList;
            TutorialPhase *currentPhase;
            detail::TutorialPlayInfoBase *instantiator;

            TutorialSequence(detail::TutorialPlayInfoBase *inst)
            : currentPhase(nullptr), instantiator(inst)
            { }

            void setupPhase(TutorialPhase &&phase);
            bool proceed(TutorialPhase *phase);
        public:
            TutorialSequence()
            : currentPhase(nullptr), instantiator(nullptr)
            { }
            TutorialSequence(const TutorialSequence &) = delete;
            TutorialSequence(TutorialSequence &&rhs)
            : phaseList(std::move(rhs.phaseList)), currentPhase(rhs.currentPhase), instantiator(rhs.instantiator)
            {
                rhs.currentPhase = nullptr;
                rhs.instantiator = nullptr;
            }

            TutorialSequence(const TutorialPhase &phase);
            TutorialSequence(TutorialPhase &&phase);
            TutorialSequence &operator>>(const TutorialPhase &phase);
            TutorialSequence &operator>>(TutorialPhase &&phase);

            TutorialSequence &operator=(const TutorialSequence &) = delete;
            TutorialSequence &operator=(TutorialSequence &&rhs)
            {
                phaseList = std::move(rhs.phaseList);

                currentPhase = rhs.currentPhase;
                rhs.currentPhase = nullptr;

                instantiator = rhs.instantiator;
                rhs.instantiator = nullptr;

                return *this;
            }

            bool proceed();
        };

        inline TutorialSequence operator>>(const TutorialPhase &lhs, const TutorialPhase &rhs)
        {
            return std::move(TutorialSequence(lhs) >> rhs);
        }
    }
}

#endif
