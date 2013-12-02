#ifndef __COCOS2D_EXT_MOVE_H__
#define __COCOS2D_EXT_MOVE_H__

#include "cocos2d.h"

namespace cocos2d
{
	class Move : public ActionInterval
	{
	private:
		Point vec, dpos;
	public:
		Move() { }

		static Move* create(const float duration, const cocos2d::Point &vec);
		bool initWithDuration(const float duration, const cocos2d::Point &vec);

		virtual void startWithTarget(Node *pTarget);
		virtual void update(float time);
		virtual void stop(void) override;

		virtual Move *clone() const override;
	};
}


#endif
