#ifndef __OREORE_UTILS_STEP_H__
#define __OREORE_UTILS_STEP_H__

#ifdef nil
#   undef nil
#endif

#include "msgpack.hpp"

#undef nil
#define nil NULL

#include <iostream>

/*
 * Step データシリアライズユーティリティ
 */

namespace oreore
{
    namespace Step
    {
        // forward
        class Encrypter;
        class Serializable;

        /**
         * ストレージ
         * 別データ形式での出力を担当するクラス
         */
        class Storage
        {
            friend bool operator>>(const Encrypter &, const Storage &);
            friend bool operator>>(const Serializable &, const Storage &);
            friend bool operator>>(const Storage &, Encrypter &);
            friend bool operator>>(const Storage &, Serializable &);
        private:

        protected:
            virtual bool store(const Encrypter &enc) const = 0;
            virtual bool store(const Serializable &data) const = 0;
            virtual bool restore(Encrypter &enc) const = 0;
            virtual bool restore(Serializable &data) const = 0;

        public:
            Storage() = default;
            virtual ~Storage() = default;
        };

        inline bool operator>>(const Encrypter &lhs, const Storage &rhs)
        {
            return rhs.store(lhs);
        }

        inline bool operator>>(const Serializable &lhs, const Storage &rhs)
        {
            return rhs.store(lhs);
        }

        inline bool operator>>(const Storage &lhs, Encrypter &rhs)
        {
            return lhs.restore(rhs);
        }

        inline bool operator>>(const Storage &lhs, Serializable &rhs)
        {
            return lhs.restore(rhs);
        }

        /**
         * シリアライズ可能クラス
         * このクラスのserialize/deserializeメソッドをオーバーライドしてください
         */
        class Serializable
        {
        private:
        public:
            Serializable() = default;
            virtual ~Serializable() = default;

            virtual bool serialize(std::ostream &out) const = 0;
            virtual bool deserialize(const std::istream &data) = 0;
        };

        /**
         * 暗号化
         * 暗号化を担当するクラス
         */
        class Encrypter
        {
            friend const Encrypter &operator>>(const Encrypter &, const Encrypter &);
            friend const Encrypter &operator>>(const Serializable &, const Encrypter &);

        private:
            inline const Encrypter &pass(const Encrypter &enc) const
            {
                encrypt(enc);
                return *this;
            }

            inline const Encrypter &pass(const Serializable &enc) const
            {
                encrypt(enc);
                return *this;
            }
        protected:
            virtual void encrypt(const Encrypter &enc) const = 0;
            virtual void encrypt(const Serializable &data) const = 0;

        public:
            Encrypter()
            { }

            virtual ~Encrypter()
            { }
        };

        inline const Encrypter &operator>>(const Encrypter &lhs, const Encrypter &rhs)
        {
            return rhs.pass(lhs);
        }

        inline const Encrypter &operator>>(const Serializable &lhs, const Encrypter &rhs)
        {
            return rhs.pass(lhs);
        }

    }
}

#endif
