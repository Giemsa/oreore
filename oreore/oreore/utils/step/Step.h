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
            bool initialized;
        public:
            Stream()
            : initialized(false)
            { }
        };

        enum class Phase
        {
            Start,
            Intermidiate,
            End
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
                ~ProcessQueue()
                {
                    for(auto *p : list)
                    {
                        delete p;
                    }
                }

                void push(ProcessBase *p)
                {
                    list.push_back(p);
                }

                bool process() const
                {
                    Stream s;
                    bool r = true;

                    switch(list.size())
                    {
                        case 0: return r;
                        case 1: return list.front()->process(s, Phase::Start);
                    }

                    r &= list.front()->process(s, Phase::Start);
                    for(int i = 1; i < list.size() - 1; ++i)
                    {
                        r &= list[i]->process(s, Phase::Intermidiate);
                    }

                    r &= list.back()->process(s, Phase::End);

                    return r;
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
                { }

                bool kick()
                {
                    const bool r = queue->process();
                    delete queue;
                    return r;
                }

                bool kick(const std::function<void(bool)> &callback)
                {
                    const bool r = queue->process();
                    delete queue;
                    return r;
                }

                bool kickAsync(const std::function<void(bool)> &callback)
                {
                    const bool r = queue->process();
                    delete queue;
                    return r;
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

                    const bool r = ProcessKicker(queue).kick();
                    queue = nullptr;
                    return r;
                }
            };

        private:
            ProcessQueue *queue;

        protected:
            virtual bool process(Stream &stream, const Phase phase) = 0;

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

                const bool r = ProcessBase::ProcessKicker(queue).kick();
                queue = nullptr;
                return r;
            }

            bool startAsync(const std::function<void(bool)> &callback)
            {
                if(!queue)
                {
                    return false;
                }

                const bool r = ProcessBase::ProcessKicker(queue).kickAsync(callback);
                queue = nullptr;
                return r;
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

        public:
            Process() = default;
            virtual ~Process() = default;

        };

        template<typename T>
        class Terminal : public Process<T>
        {
        private:
            bool process(Stream &stream, const Phase phase) override
            {
                switch(phase)
                {
                    case Phase::Start: return start(stream);
                    case Phase::End:   return end(stream);
                    default:           assert(0); return false;
                }
            }

        protected:
            virtual bool start(Stream &stream) = 0;
            virtual bool end(Stream &stream) = 0;

        public:
            Terminal() = default;
            virtual ~Terminal() = default;
        };

        template<typename T>
        class Nonterminal : public Process<T>
        {
        private:
            bool process(Stream &stream, const Phase phase) override
            {
                switch(phase)
                {
                    case Phase::Intermidiate: return process(stream);
                    default:                  assert(0); return false;
                }
            }

        protected:
            virtual bool process(Stream &stream) = 0;

        public:
            Nonterminal() = default;
            virtual ~Nonterminal() = default;
        };

        template<typename T>
        class Serializable : public Terminal<T>
        {
        private:
        public:
            Serializable() = default;
            virtual ~Serializable() = default;
        };

        template<typename T>
        using Encrypter = Nonterminal<T>;

        template<typename T>
        using Storage = Terminal<T>;
    }
}

#endif
