#include "Utils.h"
#include <ctime>

namespace oreore
{
    namespace
    {
        Random &_getRandom()
        {
            static Random rnd(std::time(0));
            return rnd;
        }
    }

    unsigned long random()
    {
        return _getRandom().next();
    }

    unsigned long random(const unsigned long max)
    {
        return _getRandom().next(max);
    }

    unsigned long random(const unsigned long min, const unsigned long max)
    {
        return _getRandom().next(min, max);
    }
}

