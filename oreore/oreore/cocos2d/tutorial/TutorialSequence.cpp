#include "TutorialSequence.h"
#include "TutorialBase.h"

namespace oreore
{
    TutorialSequence::TutorialSequence(const TutorialPhase &phase)
    : currentPhase(nullptr), instantiator(nullptr)
    {
        TutorialPhase tmp(phase);
        tmp.index = phaseList.size();
        phaseList.push_back(std::move(tmp));
    }

    TutorialSequence::TutorialSequence(TutorialPhase &&phase)
    : currentPhase(nullptr), instantiator(nullptr)
    {
        phase.index = phaseList.size();
        phaseList.push_back(std::move(phase));
    }

    TutorialSequence &TutorialSequence::operator>>(const TutorialPhase &phase)
    {
        TutorialPhase tmp(phase);
        tmp.index = phaseList.size();
        phaseList.push_back(std::move(tmp));
        return *this;
    }

    TutorialSequence &TutorialSequence::operator>>(TutorialPhase &&phase)
    {
        phase.index = phaseList.size();
        phaseList.push_back(std::move(phase));
        return *this;
    }

    bool TutorialSequence::proceed(TutorialPhase *phase)
    {
        currentPhase = phase;
        if(instantiator)
        {
            TutorialBase *tutorial = instantiator->getInstance();
            return tutorial->showTutorial([phase, tutorial]() {
                return phase->proceed(tutorial);
            });
        }

        return phase->proceed();
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
