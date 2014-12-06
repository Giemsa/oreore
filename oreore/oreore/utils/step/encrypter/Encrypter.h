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
                oreore::Blowfish browfish;

                bool process(const StepPhase &phase)
                {
                    return true;
                }

                bool process(StepPhase &phase) const
                {
                    return true;
                }

            /*
                std::vector<char> bin;

                void encrypt(const Encrypter &enc) override
                {
                }

                void encrypt(const Serializable &data) override
                {
                    std::stringstream out(std::stringstream::binary);
                    data.serialize(out);

                    std::istream_iterator<char> begin(out);
                    bin = browfish.encrypt(begin, std::istream_iterator<char>());
                };
*/
            public:
            };
        }
    }
}

#endif
