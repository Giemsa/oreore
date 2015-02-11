#include "ResizeTo.h"

namespace oreore
{
    using namespace cocos2d;

    ResizeTo *ResizeTo::create(const float duration, const Size &size)
    {
        ResizeTo *action = new ResizeTo();
        if(action && action->initWithDuration(duration, size))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return nullptr;
    }

    bool ResizeTo::initWithDuration(const float duration, const Size &size)
    {
        if(!ActionInterval::initWithDuration(duration))
        {
            return false;
        }

        size_ = size;
        return true;
    }

    void ResizeTo::startWithTarget(Node *target)
    {
        size_ = size_ - target->getContentSize();
        ResizeBy::startWithTarget(target);
    }

    ResizeTo *ResizeTo::reverse() const
    {
        CCASSERT(0, "reverse is not implemented");
        return nullptr;
    }

    ResizeTo *ResizeTo::clone() const
    {
        return ResizeTo::create(getDuration(), size_);
    }
}

