#ifndef __OREORE_UTILS_STEP_H__
#define __OREORE_UTILS_STEP_H__

#ifdef nil
#   undef nil
#endif

#include "msgpack.hpp"

#undef nil
#define nil NULL

#include <iostream>
#include <type_traits>

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
        class StepPhase;

        namespace detail
        {
            template<typename T, typename N>
            class has_member_impl
            {
                template<typename C>
                static char check(typename N::template get<C> *);

                template<typename C>
                static int check(...);

            public:
                static const bool value = (sizeof(check<T>(nullptr)) == sizeof(char));
            };

            template<typename T, typename N>
            struct has_member
            : std::integral_constant<bool, detail::has_member_impl<T, N>::value>
            { };

            /* method checker */
            struct check_has_store_impl
            {
                template<
                    typename T,
                    void (T::*)(typename T::ResultType &, const StepPhase &) = &T::process
                >
                struct get { };
            };

            template<typename T>
            struct check_has_store
            : has_member<T, check_has_store_impl>
            { };
        }

        class StepPhase
        {
            friend class Serializable;
            friend class Encrypter;
            friend class Storage;

        public:
            using StreamType = std::stringstream;

        private:
            StreamType *buf;
            inline void clearBuf() const { delete buf; }

        public:
            virtual bool process(const StepPhase &phase) = 0;
            virtual bool process(StepPhase &phase) const = 0;

            StepPhase()
            : buf(nullptr)
            { }

            virtual ~StepPhase() = default;

            StreamType &getBuf()
            {
                if(!buf)
                {
                    buf = new StreamType();
                }

                return *buf;
            }

            inline const StreamType &getBuf() const { return *buf; }
        };

        /**
         * ストレージ
         * 別データ形式での出力を担当するクラス
         */
        class Storage : public StepPhase
        {
            friend class Serializable;
        public:
            using ResultType = bool;

        private:
            template<
                typename T,
                typename S
            >
            inline auto callStore(const S &t)
                -> const typename std::enable_if<detail::check_has_store<T>::value, T>::type::ResultType
            {
                typename T::ResultType result;
                static_cast<T *>(this)->process(result, t);
                t.clearBuf();
                return result;
            }

            template<
                typename T,
                typename S
            >
            inline auto callStore(const S &t)
                -> typename std::enable_if<!detail::check_has_store<T>::value, bool>::type
            {
                const bool r = process(t);
                t.clearBuf();
                return r;
            }

        public:
            Storage() = default;
            virtual ~Storage() = default;

            inline bool operator>>(Serializable &data) const;
            inline bool operator>>(Serializable &&data) const;
            inline bool operator>>(Encrypter &enc) const;
            inline bool operator>>(Encrypter &&enc) const;
        };

        /**
         * シリアライズ可能クラス
         * このクラスのserialize/deserializeメソッドをオーバーライドしてください
         */
        class Serializable : public StepPhase
        {
            friend class Storage;

        private:
            inline bool process(const StepPhase &phase) override
            {
                const bool r = deserialize(getBuf());
                phase.clearBuf();
                return r;
            }

            inline bool process(StepPhase &phase) const override
            {
                return serialize(phase.getBuf());
            }

        public:
            Serializable() = default;
            virtual ~Serializable() = default;

            /* override these methods */
            virtual bool serialize(std::ostream &out) const = 0;
            virtual bool deserialize(const std::istream &data) = 0;

            /* operator overload */
            inline Encrypter &&operator>>(Encrypter &enc) const
            {
                return std::move(enc);
            }

            inline Encrypter &&operator>>(Encrypter &&enc) const
            {
                return std::move(enc);
            }

            template<
                typename T,
                typename = typename std::enable_if<std::is_base_of<Storage, T>::value>::type
            >
            inline const typename T::ResultType operator>>(T &storage) const
            {
                process(storage);
                return storage.template callStore<T>(*this);
            }

            template<
                typename T,
                typename = typename std::enable_if<std::is_base_of<Storage, T>::value>::type
            >
            inline const typename T::ResultType operator>>(T &&storage) const
            {
                process(storage);
                return storage.template callStore<T>(*this);
            }
        };

        /**
         * 暗号化
         * 暗号化を担当するクラス
         */
        class Encrypter : public StepPhase
        {
        private:

        public:
            Encrypter()
            { }

            virtual ~Encrypter()
            { }

            inline bool operator>>(StepPhase &phase) const
            {
                return process(phase);
            }

            inline bool operator>>(StepPhase &&phase) const
            {
                return process(phase);
            }
        };

        /* Storage impl */
        inline bool Storage::operator>>(Serializable &data) const
        {
            const bool r = process(data) && data.process(*this);
            data.clearBuf();
            return r;
        }

        inline bool Storage::operator>>(Serializable &&data) const
        {
            const bool r = process(data) && data.process(*this);
            data.clearBuf();
            return r;
        }

        inline bool Storage::operator>>(Encrypter &enc) const { return process(enc); }
        inline bool Storage::operator>>(Encrypter &&enc) const { return process(enc); }

    }
}

#endif
