#ifndef __OREORE_UTILS_STEP_ENCRYPTER_H__
#define __OREORE_UTILS_STEP_ENCRYPTER_H__

#include "../../../libs/blowfish/blowfish.h"
#include "../Step.h"

namespace oreore
{
    namespace Step
    {
        namespace Encrypt
        {
            /**
             * Blowfishを用いた共通鍵暗号を施す
             * もちろん実装中
             */
            class Blowfish final : public Encrypter
            {
            private:
                void encrypt(const Encrypter &enc) const override
                {
                }

                void encrypt(const Serializable &data) const override
                {
                };

            public:
            };
        }
    }
}

#endif
