#include "Utils.h"
#include <ctime>

namespace oreore
{
	unsigned long random()
	{
		static Random rnd(std::time(0));
		return rnd.next();
	}
}