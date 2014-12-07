#ifndef __OREORE_STEP_SERIALIZER_MSGPACK_H__
#define __OREORE_STEP_SERIALIZER_MSGPACK_H__

#include "../Step.h"

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
        class MessagePack : public Serializable
        {
        private:
            bool serialize(std::ostream &stream) const override
            {
                std::cout << "MessagePack start" << std::endl;
                return true;
            }

            bool deserialize(const std::istream &stream) override
            {
                std::cout << "MessagePack end" << std::endl;
                return true;
            }

        public:
            MessagePack() = default;
            virtual ~MessagePack() = default;
        };
    }
}

#endif
