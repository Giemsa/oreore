#ifndef __OREORE_COCOS2D_MULTIRESOLITION_H__
#define __OREORE_COCOS2D_MULTIRESOLITION_H__

#include "cocos2d.h"

namespace oreore
{
	struct Resolution
	{
		cocos2d::Size size;
		char directory[64];
	};

	class MultiResolution
	{
	private:
	public:
		MultiResolution() { }
		~MultiResolution() { }
	};
}

#endif
