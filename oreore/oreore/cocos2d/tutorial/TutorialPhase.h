#ifndef __OREORE_TUTORIAL_SEQUENCE_H__
#define __OREORE_TUTORIAL_SEQUENCE_H__

#include <vector>
#include <functional>

namespace oreore
{
    namespace Tutorial
    {
        // forward
        namespace detail
        {
            class TutorialBaseBase; // in TutorialBase.h
        }

        enum class PhaseType
        {
            Stay,  // 通常状態。デフォルト値
            Begin, // フェードインしながらチュートリアル再生開始
            End    // フェードアウトしながらチュートリアル終了
        };

        class TutorialPhase
        {
        public:
            using func_type = std::function<bool(const size_t)>;
        private:
            func_type proc;
            TutorialPhase *next;
            int trigger;
            bool played;
            bool tapToNext;
            PhaseType type;
            detail::TutorialBaseBase *base;
        public:
            TutorialPhase()
            : next(nullptr)
            , trigger(0)
            , played(false)
            , tapToNext(false)
            , type(PhaseType::Stay)
            , base(nullptr)
            { }

            TutorialPhase(const func_type &proc, detail::TutorialBaseBase *base)
            : proc(proc)
            , next(nullptr)
            , trigger(0)
            , played(false)
            , tapToNext(true)
            , type(PhaseType::Stay)
            , base(base)
            { }

            TutorialPhase(const int trigger, const func_type &proc, detail::TutorialBaseBase *base)
            : proc(proc)
            , next(nullptr)
            , trigger(trigger)
            , played(false)
            , tapToNext(false)
            , type(PhaseType::Stay)
            , base(base)
            { }

            TutorialPhase(const int trigger, const func_type &proc, const PhaseType type, detail::TutorialBaseBase *base)
            : proc(proc)
            , next(nullptr)
            , trigger(trigger)
            , played(false)
            , tapToNext(false)
            , type(type)
            , base(base)
            { }

            TutorialPhase(const TutorialPhase &rhs)
            : proc(rhs.proc)
            , next(rhs.next)
            , trigger(rhs.trigger)
            , played(rhs.played)
            , tapToNext(rhs.tapToNext)
            , type(rhs.type)
            , base(rhs.base)
            { }

            TutorialPhase(TutorialPhase &&rhs)
            : proc(rhs.proc)
            , next(rhs.next)
            , trigger(rhs.trigger)
            , played(rhs.played)
            , tapToNext(rhs.tapToNext)
            , type(rhs.type)
            , base(rhs.base)
            {
                rhs.proc = nullptr;
                rhs.next = nullptr;
                rhs.base = nullptr;
            }

            bool proceed(const size_t index);

            TutorialPhase &operator=(const TutorialPhase &) = delete;
            TutorialPhase &operator=(TutorialPhase &&rhs)
            {
                proc = rhs.proc;
                rhs.proc = nullptr;

                next = rhs.next;
                rhs.next = nullptr;

                trigger = rhs.trigger;
                played = rhs.played;
                tapToNext = rhs.tapToNext;
                type = rhs.type;

                base = rhs.base;
                rhs.base = nullptr;

                return *this;
            }

            int getTrigger() const { return trigger; }
            bool isPlayed() const { return played; }
            PhaseType getType() const { return type; }
            void setType(const PhaseType t) { type = t; }
        };
    }
}

#endif
