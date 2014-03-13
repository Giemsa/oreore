#ifndef __OREORE_COCOS2D_DEBUG_H__
#define __OREORE_COCOS2D_DEBUG_H__

#include "cocos2d.h"
#include <vector>
#include "../../null.h"

namespace oreore
{
    class DebugLayer : public cocos2d::CCLayer
    {
    private:
    public:
        CREATE_FUNC(DebugLayer);
        bool init(); // override

        void visit(); // override
    };
}

#endif
