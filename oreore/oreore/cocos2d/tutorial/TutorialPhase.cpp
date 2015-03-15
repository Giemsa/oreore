#include "TutorialPhase.h"
#include "TutorialBase.h"

namespace oreore
{
    namespace Tutorial
    {
        bool TutorialPhase::proceed(const size_t index)
        {
            switch(type)
            {
                case PhaseType::Begin:
                    return base->showTutorial([this, index]() {
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
