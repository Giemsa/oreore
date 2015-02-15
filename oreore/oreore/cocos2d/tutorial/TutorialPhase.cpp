#include "TutorialPhase.h"
#include "TutorialBase.h"

namespace oreore
{
    namespace Tutorial
    {
        bool TutorialPhase::proceed()
        {
            switch(type)
            {
                case PhaseType::Begin:
                    return base->showTutorial([this]() {
                        played = true;
                        return proc(index);
                    });

                case PhaseType::Stay:
                    played = true;
                    return proc(index);

                case PhaseType::End:
                    base->hideTutorial();
                    played = true;
                    return proc(index);
            }
        }
    }
}
