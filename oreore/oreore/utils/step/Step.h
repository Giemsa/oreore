#ifndef __OREORE_UTILS_STEP_H__
#define __OREORE_UTILS_STEP_H__

#ifdef nil
#   undef nil
#endif

#include "msgpack.hpp"

#undef nil
#define nil NULL

#include <iostream>
#include <functional>

/*
 * Step データシリアライズユーティリティ
 */

namespace oreore
{
    namespace Step
    {
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
