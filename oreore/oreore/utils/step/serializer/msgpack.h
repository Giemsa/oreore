#ifndef __OREORE_STEP_SERIALIZER_MSGPACK_H__
#define __OREORE_STEP_SERIALIZER_MSGPACK_H__

#include "../Step.h"

namespace oreore
{
    namespace Step
    {
        class MessagePack final : public Serializable<MessagePack>
        {
        private:
            bool start(Stream &stream) override
            {
                std::cout << "MessagePack start" << std::endl;
                return true;
            }

            bool end(Stream &stream) override
            {
                std::cout << "MessagePack end" << std::endl;
                return true;
            }

        public:
            MessagePack() = default;
            ~MessagePack() = default;
        };
    }
}

#endif
