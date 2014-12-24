#ifndef __OREORE_UTILS_STEP_H__
#define __OREORE_UTILS_STEP_H__

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
        public:
            using StreamType = std::stringstream;
        private:
            StreamType ss;
        public:
            Stream()
            : ss(std::stringstream::in | std::stringstream::out | std::ios::binary)
            { }

            inline StreamType &getStream() { return ss; }
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

                inline operator bool()
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

            virtual ~ProcessBase() { }

            virtual ProcessBase *clone() const = 0;
            virtual void destroy() { }

            inline ProcessQueueContainer operator>>(const ProcessBase &process)
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

            ProcessHolder(ProcessBase::ProcessQueueContainer &&container)
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

        /**
         * プロセス
         * Stepに置けるフェーズを表すクラス。
         */
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
            virtual ~Process() { }

        };

        /**
         * 終端フェーズの基底クラス
         * 開始フェーズ、終了フェーズに呼ぶことの出来るプロセスを定義します。
         * 中間フェーズにこのプロセスを配置することは出来ません。
         */
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
            virtual ~Terminal() { }
        };

        /**
         * 非終端フェーズの基底クラス
         * 中間フェーズに呼ぶことの出来るプロセスを定義します。
         * 開始フェーズ、終了フェーズこのプロセスを配置することは出来ません。
         */
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
            virtual ~Nonterminal() { }
        };

        /**
         * シリアライズ可能クラス
         * このクラスを継承したクラスは、Serializerによって
         * シリアライズ/デシリアライズされます。
         */
        class Serializable
        {
            friend class Serializer;
        protected:
            virtual bool serialize(std::ostream &stream) const = 0;
            virtual bool deserialize(const std::istream &stream) = 0;

        public:
            Serializable() = default;
            virtual ~Serializable() { }
        };

        /**
         * シリアライザ
         * Serializableを継承したクラスを指定することで、
         * 指定されたクラスをシリアライザ/デシリアライズします。
         */
        class Serializer final : public Terminal<Serializer>
        {
        private:
            Serializable &data;

            inline bool start(Stream &stream) override
            {
                return data.serialize(stream.getStream());
            }

            inline bool end(Stream &stream) override
            {
                return data.deserialize(stream.getStream());
            }
        public:
            Serializer(Serializable &data)
            : data(data)
            { }
        };

        /**
         * 暗号化方式クラス
         * このクラスを継承したクラスは、Encrypter及びDecrypterによって
         * 暗号化/復号化されます。
         */
        class Cipher
        {
            friend class Encrypter;
            friend class Decrypter;
        protected:
            virtual bool encrypt(Stream::StreamType &stream) const = 0;
            virtual bool decrypt(Stream::StreamType &stream) const = 0;

        public:
            Cipher() = default;
            virtual ~Cipher() { }
        };

        /**
         * 暗号化クラス
         * Cipherを継承したクラスを指定することで、
         * ストリームの内容を指定した暗号化方式で暗号化します。
         */
        class Encrypter final : public Nonterminal<Encrypter>
        {
        private:
            const Cipher &cipher;

            inline bool process(Stream &stream) override
            {
                return cipher.encrypt(stream.getStream());
            }

        public:
            Encrypter(const Cipher &cipher)
            : cipher(cipher)
            { }
        };

        /**
         * 復号化クラス
         * Cipherを継承したクラスを指定することで、
         * ストリームの内容を指定した暗号化方式で復号化します。
         */
        class Decrypter final : public Nonterminal<Decrypter>
        {
        private:
            const Cipher &cipher;

            inline bool process(Stream &stream) override
            {
                return cipher.decrypt(stream.getStream());
            }

        public:
            Decrypter(const Cipher &cipher)
            : cipher(cipher)
            { }
        };

        /**
         * ストレージクラス
         * ストリームの内容を別のデータ形式やファイルへ保存します。
         */
        template<typename T>
        using Storage = Terminal<T>;
    }
}

#endif
