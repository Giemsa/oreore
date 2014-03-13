#ifndef __OREORE_COCOS2D_DEBUG_H__
#define __OREORE_COCOS2D_DEBUG_H__

#include "cocos2d.h"
#include <vector>
#include "../../null.h"

namespace oreore
{
    class DebugLayer : public cocos2d::Layer
    {
    private:
    protected:
        bool init(); // override
    public:
        CREATE_FUNC(DebugLayer);

        void visit(); // override
    };
}

#endif
