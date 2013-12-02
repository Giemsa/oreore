#include "Move.h"
#include "cocos2d.h"
#include "../Utils.h"


namespace cocos2d
{
	Move *Move::create(const float duration, const Point &vec)
	{
		Move *action = new Move();
		action->initWithDuration();
		action->autorelease();
		return action;
	}

	bool Move::initWithDuration(const float duration, const Point &vec)
	{
		if(ActionInterval::initWithDuration(duration))
		{
			this->vec = vec;
			return true;
		}

		return false;
	}

	void Move::update(float time)
	{
		getOriginalTarget()->setPosition(dpos + vec);
	}

	void Move::startWithTarget(Node *target)
	{
		ActionInterval::startWithTarget(target);
		dpos = target->getPosition();
	}

	void Move::stop(void)
	{
		getOriginalTarget()->setPosition(dpos);
		ActionInterval::stop();
	}

	Move *Move::clone() const
	{
		return Move::create(getDuration(), vec);
	}
}
