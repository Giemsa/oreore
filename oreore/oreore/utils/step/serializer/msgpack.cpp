#include "msgpack.h"


#ifdef nil
#   undef nil
#endif

#include "msgpack.hpp"

#undef nil
#define nil NULL


namespace oreore
{
    namespace Step
    {
        bool MessagePack::serialize(std::ostream &stream) const
        {
            std::cout << "MessagePack start" << std::endl;
            return true;
        }

        bool MessagePack::deserialize(const std::istream &stream)
        {
            std::cout << "MessagePack end" << std::endl;
            return true;
        }
}
}