#include "TutorialSequence.h"
#include "TutorialBase.h"

namespace oreore
{
    namespace Tutorial
    {
        TutorialSequence::TutorialSequence(const TutorialPhase &phase)
        : currentPhase(nullptr), tutorial(nullptr)
        {
            TutorialPhase tmp(phase);
            setupPhase(std::move(tmp));
        }

        TutorialSequence::TutorialSequence(TutorialPhase &&phase)
        : currentPhase(nullptr), tutorial(nullptr)
        {
            setupPhase(std::move(phase));
        }

        TutorialSequence &TutorialSequence::operator>>(const TutorialPhase &phase)
        {
            TutorialPhase tmp(phase);
            setupPhase(std::move(tmp));
            return *this;
        }

        TutorialSequence &TutorialSequence::operator>>(TutorialPhase &&phase)
        {
            setupPhase(std::move(phase));
            return *this;
        }

        TutorialSequence &TutorialSequence::operator>>(const TutorialPhase::func_type &phase)
        {
            TutorialPhase tmp(phase);
            setupPhase(std::move(tmp));
            return *this;
        }

        TutorialSequence &TutorialSequence::operator>>(TutorialPhase::func_type &&phase)
        {
            setupPhase(std::move(phase));
            return *this;
        }

        void TutorialSequence::setupPhase(TutorialPhase &&phase)
        {
            phase.index = phaseList.size();
            TutorialPhase *prev = phaseList.empty() ? nullptr : &phaseList.back();
            phaseList.push_back(std::move(phase));
            if(prev)
            {
                prev->next = &phaseList.back();
            }
        }

        bool TutorialSequence::proceed(TutorialPhase *phase)
        {
            currentPhase = phase;
            return tutorial->showTutorial(this, [phase]() {
                return phase->proceed();
            });
        }

        bool TutorialSequence::proceed()
        {
            if(!currentPhase)
            {
                if(phaseList.empty())
                {
                    return false;
                }

                return proceed(&phaseList.front());
            }

            if(!currentPhase->next)
            {
                return false;
            }

            return proceed(currentPhase->next);
        }
    }
}
