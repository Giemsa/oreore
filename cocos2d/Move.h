#ifndef __COCOS2D_EXT_MOVE_H__
#define __COCOS2D_EXT_MOVE_H__

#include "cocos2d.h"

namespace cocos2d
{
	class Move : public ActionInterval
	{
	private:
		Point vec;
	public:
		Move() { }

		static Move* create(const float duration, const Point &vec);
		bool initWithDuration(const float duration, const Point &vec);

		virtual void startWithTarget(Node *pTarget);
		virtual void update(float time);
		virtual void stop(void) override;

		virtual Move *reverse() const override;
		virtual Move *clone() const override;
	};
}


#endif
