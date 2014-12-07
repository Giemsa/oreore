#ifndef __OREORE_STEP_PROCESS_ENCRYPT_BLOWFISH_H__
#define __OREORE_STEP_PROCESS_ENCRYPT_BLOWFISH_H__

#include "../../Step.h"
#include "../../../../libs/blowfish/blowfish.h"

namespace oreore
{
    namespace Step
    {
        namespace Encrypt
        {
            class Blowfish final : public Cipher
            {
                using CharVector = std::vector<char>;
            private:
                const oreore::Blowfish blowfish;

                bool encrypt(Stream::StreamType &stream) const
                {
                    const CharVector &buf = blowfish.encrypt(
                        std::istream_iterator<char>(stream),
                        std::istream_iterator<char>()
                    );
                    stream.str("");
                    stream.clear();
                    std::copy(buf.begin(), buf.end(), std::ostream_iterator<char>(stream));
                    return true;
                }

                bool decrypt(Stream::StreamType &stream) const
                {
                    const CharVector &buf = blowfish.decrypt(
                        std::istream_iterator<char>(stream),
                        std::istream_iterator<char>()
                    );
                    stream.str("");
                    stream.clear();
                    std::copy(buf.begin(), buf.end(), std::ostream_iterator<char>(stream));
                    return true;
                }

            public:
                Blowfish(const std::string &key)
                : blowfish(key)
                { }

                ~Blowfish() = default;
            };
        }
    }
}

#endif

