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

namespace oreore
{
    class Blowfish
    {
    public:
        Blowfish(const std::vector<char> &key);
        std::vector<char> encrypt(const std::vector<char> &src) const;
        std::vector<char> decrypt(const std::vector<char> &src) const;

    private:
        uint32_t pary_[18];
        uint32_t sbox_[4][256];

        void setKey(const char *key, const size_t byte_length);
        void encryptBlock(uint32_t *left, uint32_t *right) const;
        void decryptBlock(uint32_t *left, uint32_t *right) const;
        uint32_t feistel(const uint32_t value) const;
    };
}

#endif
