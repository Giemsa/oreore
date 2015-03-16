#ifndef __OREORE_STEP_SERIALIZER_MSGPACK_H__
#define __OREORE_STEP_SERIALIZER_MSGPACK_H__

#include "../Step.h"

namespace oreore
{
    namespace Step
    {
        /**
         * MessagePackシリアライザ
         * このクラスを継承し、serialize/deserializeを継承することで、
         * 任意のデータをMessagePack形式にシリアライズすることができます。
         */
        class MessagePack : public Serializable
        {
        private:
            bool serialize(std::ostream &stream) const override;
            bool deserialize(const std::istream &stream) override;

        public:
            MessagePack() = default;
            virtual ~MessagePack() { }
        };
    }
}

#endif
