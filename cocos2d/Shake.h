#ifndef __COCOS2D_EXT_SHAKE_H__
#define __COCOS2D_EXT_SHAKE_H__

#include "cocos2d.h"

namespace cocos2d
{
	class Shake : public ActionInterval
	{
	private:
		Point dpos;
		float strength_x, strength_y;
	public:
		Shake();

		static Shake* create(float duration, float level);
		static Shake* create(float duration, float level_x, float level_y);
		bool initWithDuration(float duration, float level_x, float level_y);

		virtual void startWithTarget(Node *pTarget);
		virtual void update(float time);
		virtual void stop(void) override;
        
        virtual Shake *reverse() const override;
        virtual Shake *clone() const override;
	};
}


#endif
