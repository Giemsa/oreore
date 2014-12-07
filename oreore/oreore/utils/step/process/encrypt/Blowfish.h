#ifndef __OREORE_STEP_PROCESS_ENCRYPT_BLOWFISH_H__
#define __OREORE_STEP_PROCESS_ENCRYPT_BLOWFISH_H__

#include "../../Step.h"

namespace oreore
{
    namespace Step
    {
        namespace Encrypt
        {
            class Blowfish final : public Encrypter<Blowfish>
            {
            private:
                bool process(Stream &stream) override
                {
                    std::cout << "Blowfish process" << std::endl;
                    return true;
                }

            public:
                Blowfish() = default;
                ~Blowfish() = default;
            };
        }
    }
}

#endif

