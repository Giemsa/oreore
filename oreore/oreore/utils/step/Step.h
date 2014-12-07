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
#include <functional>

/*
 * Step データシリアライズユーティリティ
 */

namespace oreore
{
    namespace Step
    {

#if 0
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
#endif
        class Stream final
        {
        private:
        public:
        };

        class ProcessBase
        {
            friend class ProcessHolder;

            class ProcessQueue final
            {
                using List = std::vector<ProcessBase *>;
            private:
                List list;
            public:
                void push(ProcessBase *p)
                {
                    list.push_back(p);
                }
            };

            class ProcessKicker final
            {
            private:
                const ProcessQueue *queue;
            public:
                ProcessKicker() { }
                ProcessKicker(const ProcessQueue *queue)
                : queue(queue)
                {
                    std::cout << "created" << std::endl;
                }

                void kick()
                {
                    std::cout << "kick" << std::endl;
                    delete queue;
                }

                void kick(const std::function<void(bool)> &callback)
                {
                    std::cout << "kick with callback" << std::endl;
                    delete queue;
                }

                void kickAsync(const std::function<void(bool)> &callback)
                {
                    std::cout << "async kick with callback" << std::endl;
                    delete queue;
                }
            };

            class ProcessQueueContainer final
            {
                friend class ProcessHolder;
            private:
                ProcessQueue *queue;

            public:
                ProcessQueueContainer(ProcessQueue *queue)
                : queue(queue)
                { }

                ~ProcessQueueContainer()
                {
                    if(queue)
                    {
                        ProcessKicker(queue).kick();
                    }
                }

                inline ProcessQueueContainer &operator>>(const ProcessBase &process)
                {
                    queue->push(process.clone());
                    return *this;
                }

                bool startAsync()
                {
                    if(!queue)
                    {
                        return false;
                    }

                    ProcessKicker(queue).kick();
                    queue = nullptr;
                    return true;
                }
            };

        private:
            ProcessQueue *queue;

        public:
            ProcessBase()
            : queue(nullptr)
            { }
            
            ProcessBase(ProcessQueue *queue)
            : queue(queue)
            { }

            virtual ~ProcessBase() = default;

            virtual ProcessBase *clone() const = 0;

            inline ProcessQueueContainer operator>>(const ProcessBase &process) const
            {
                ProcessQueue *q = new ProcessQueue();
                q->push(clone());
                q->push(process.clone());
                return ProcessQueueContainer(q);
            }
        };

        class ProcessHolder final
        {
        private:
            ProcessBase::ProcessQueue *queue;
            ProcessHolder() = delete;

            ProcessHolder(const ProcessHolder &rhs) = delete;

        public:
            ProcessHolder(ProcessBase::ProcessQueueContainer &container)
            : queue(container.queue)
            {
                container.queue = nullptr;
            }

            ProcessHolder(ProcessHolder &&rhs)
            {
                queue = rhs.queue;
                rhs.queue = nullptr;
            }

            ~ProcessHolder()
            {
                if(queue)
                {
                    ProcessBase::ProcessKicker(queue).kick();
                }
            }

            ProcessHolder &operator=(ProcessHolder &rhs)
            {
                queue = rhs.queue;
                rhs.queue = nullptr;
                return *this;
            }

            bool start()
            {
                if(!queue)
                {
                    return false;
                }

                ProcessBase::ProcessKicker(queue).kick();
                queue = nullptr;
                return true;
            }

            bool startAsync(const std::function<void(bool)> &callback)
            {
                if(!queue)
                {
                    return false;
                }

                ProcessBase::ProcessKicker(queue).kickAsync(callback);
                queue = nullptr;
                return true;
            }
        };

        template<typename T>
        class Process : public ProcessBase
        {
        private:
            inline ProcessBase *clone() const override
            {
                return new T(*static_cast<const T *>(this));
            }

        protected:
            virtual bool process(Stream &stream) = 0;

        public:
            Process() = default;
            virtual ~Process() = default;

        };

        template<typename T>
        class Serializable : public Process<T>
        {
        private:
        public:
            Serializable() = default;
            virtual ~Serializable() = default;
        };

        template<typename T>
        class Encrypter : public Process<T>
        {
        private:
        public:
            Encrypter() = default;
            virtual ~Encrypter() = default;
        };

        template<typename T>
        class Storage : public Process<T>
        {
        private:
        public:
            Storage() = default;
            virtual ~Storage() = default;
        };

        /* test */
        class JSON final : public Serializable<JSON>
        {
        private:
            bool process(Stream &stream) override
            {
                return true;
            }

        public:
            JSON() = default;
            ~JSON() = default;
        };

        class StringStorage final : public Storage<StringStorage>
        {
        private:
            bool process(Stream &stream) override
            {
                return true;
            }

        public:
            StringStorage() = default;
            ~StringStorage() = default;
        };

        class Blowfish final : public Encrypter<Blowfish>
        {
        private:
            bool process(Stream &stream) override
            {
                return true;
            }

        public:
            Blowfish() = default;
            ~Blowfish() = default;
        };
    }
}

#endif
