#ifndef __OREORE_STEP_SERIALIZER_JSON_H__
#define __OREORE_STEP_SERIALIZER_JSON_H__

#include "../Step.h"

namespace oreore
{
    namespace Step
    {
        class JSON final : public Serializable<JSON>
        {
        private:
            bool start(Stream &stream) override
            {
                std::cout << "JSON start" << std::endl;
                return true;
            }

            bool end(Stream &stream) override
            {
                std::cout << "JSON end" << std::endl;
                return true;
            }

        public:
            JSON() = default;
            ~JSON() = default;
        };
    }
}

#endif
