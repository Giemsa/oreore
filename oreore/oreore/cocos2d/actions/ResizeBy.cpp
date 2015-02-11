#include "ResizeBy.h"

namespace oreore
{
    using namespace cocos2d;

    ResizeBy *ResizeBy::create(const float duration, const Size &size)
    {
        ResizeBy *action = new ResizeBy();
        if(action && action->initWithDuration(duration, size))
        {
            action->autorelease();
            return action;
        }
        delete action;
        return nullptr;
    }

    bool ResizeBy::initWithDuration(const float duration, const Size &size)
    {
        if(!ActionInterval::initWithDuration(duration))
        {
            return false;
        }

        size_ = size;
        return true;
    }

    void ResizeBy::startWithTarget(Node *target)
    {
        ActionInterval::startWithTarget(target);
        baseSize = target->getContentSize();
    }

    void ResizeBy::update(float time)
    {
        getOriginalTarget()->setContentSize(
            baseSize + size_ * time
        );
    }

    ResizeBy *ResizeBy::reverse() const
    {
        CCAssert(0, "reverse is not implemented");
        return nullptr;
    }

    ResizeBy *ResizeBy::clone() const
    {
        return ResizeBy::create(getDuration(), size_);
    }
}

