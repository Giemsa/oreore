#ifndef __OREORE_LIBS_BLOWFISH_H__
#define __OREORE_LIBS_BLOWFISH_H__

/**
 * Blowfish cipher
 *
 * https://github.com/h2so5/Blowfish
 */

#include <stdint.h>
#include <cstddef>
#include <vector>
#include <string>

namespace oreore
{
    class Blowfish
    {
    public:
        Blowfish(const std::vector<char> &key);
        Blowfish(const std::string &key);

        template<typename Itr>
        std::vector<char> encrypt(const Itr &begin, const Itr &end) const
        {
            std::vector<char> dst(begin, end);

            size_t padding_length = dst.size() % sizeof(uint64_t);
            if(padding_length == 0)
            {
                padding_length = sizeof(uint64_t);
            }
            else
            {
                padding_length = sizeof(uint64_t) - padding_length;
            }

            for(size_t i = 0; i < padding_length; ++i)
            {
                dst.push_back(static_cast<char>(padding_length));
            }

            for(int i = 0; i < dst.size() / sizeof(uint64_t); ++i)
            {
                encryptBlock(
                    &reinterpret_cast<uint32_t *>(dst.data())[i * 2],
                    &reinterpret_cast<uint32_t *>(dst.data())[i * 2 + 1]
                );
            }

            return dst;
        }

        template<typename Itr>
        std::vector<char> decrypt(const Itr &begin, const Itr &end) const
        {
            std::vector<char> dst(begin, end);

            for(int i = 0; i < dst.size() / sizeof(uint64_t); ++i)
            {
                decryptBlock(
                    &reinterpret_cast<uint32_t *>(dst.data())[i * 2],
                    &reinterpret_cast<uint32_t *>(dst.data())[i * 2 + 1]
                );
            }

            const size_t padding_length = PKCS5PaddingLength(dst);
            dst.resize(dst.size() - padding_length);
            return dst;
        }

        inline std::vector<char> encrypt(const std::vector<char> &src) const
        {
            return encrypt(src.begin(), src.end());
        }

        inline std::vector<char> decrypt(const std::vector<char> &src) const
        {
            return decrypt(src.begin(), src.end());
        }

    private:
        uint32_t pary_[18];
        uint32_t sbox_[4][256];

        void setKey(const char *key, const size_t byte_length);
        void encryptBlock(uint32_t *left, uint32_t *right) const;
        void decryptBlock(uint32_t *left, uint32_t *right) const;
        uint32_t feistel(const uint32_t value) const;
        size_t PKCS5PaddingLength(const std::vector<char> &data) const;
    };
}

#endif
