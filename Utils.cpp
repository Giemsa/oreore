#include "Utils.h"
#include <ctime>


namespace Utils
{
	unsigned long random()
	{
		static Random rnd(std::time(0));
		return rnd.next();
	}
}