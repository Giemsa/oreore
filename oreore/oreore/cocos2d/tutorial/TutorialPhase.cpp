#include "TutorialPhase.h"
#include "TutorialBase.h"

namespace oreore
{
    namespace Tutorial
    {
        bool TutorialPhase::proceed()
        {
            using namespace oreore;

            if(type == PhaseType::Begin)
            {
                return base->showTutorial(this, [this]() {
                    played = true;
                    return proc(index);
                });
            }

            played = true;
            return proc(index);
        }
    }
}
