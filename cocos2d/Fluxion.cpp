#include "Fluxion.h"


namespace oreore
{
    namespace Fluxion
    {
        /* Action */
        Action &Action::operator&&const Action &action)
        {
            return *this;
        }
        
        Action::operator cocos2d::Action *()
        {
            return null;
        }

        /* SequentialAction */
        SequentialAction::operator cocos2d::Action *()
        {
            return null;
        }

        /* ParallelAction */
        ParallelAction::operator cocos2d::Action *()
        {
            return null;
        }
        
    }
    
    namespace x
    {
        /* MoveTo */
        
    }
}
