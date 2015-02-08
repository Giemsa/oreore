#ifndef MW_UTIL_BUFFER_HPP
#define MW_UTIL_BUFFER_HPP

#include <cstdlib>

namespace utils
{
    template<typename T>
    class buffer final
    {
        class internal_buffer
        {
        private:
            size_t size_, index_;
            internal_buffer *next_;

            internal_buffer(const size_t size)
            : size_(size), index_(0), next_(nullptr)
            { }
        public:
            static internal_buffer *create(const size_t size)
            {
                return new(std::malloc(sizeof(internal_buffer) + sizeof(T) * size)) internal_buffer(size);
            }

            static void destroy(internal_buffer *p)
            {
                while(p)
                {
                    internal_buffer *tmp = p->next_;
                    p->~internal_buffer();
                    std::free(p);
                    p = tmp;
                }
            }

            T *get_buffer() { return reinterpret_cast<T *>(this + 1); }
            const T *get_buffer() const { return reinterpret_cast<const T *>(this + 1); }

            bool push_back(T &&elem)
            {
                if(index_ == size_)
                {
                    return false;
                }

                new(get_buffer() + index_++) T(std::move(elem));
                return true;
            }

            void setNext(internal_buffer *next)
            {
                next_ = next;
            }

            T &front() { return *get_buffer(); }
            const T &front() const { return *get_buffer(); }
            T &back() { return *(get_buffer() + index_ - 1); }
            const T &back() const { return *(get_buffer() + index_ - 1); }
        };

    private:
        internal_buffer *head_, *buf_;
        size_t size_, bufsize_;

        void init()
        {
            head_ = buf_ = internal_buffer::create(bufsize_);
        }
    public:
        buffer()
        : head_(nullptr), buf_(nullptr), size_(0), bufsize_(8)
        {
            init();
        }

        buffer(const size_t bufsize)
        : head_(nullptr), buf_(nullptr), size_(0), bufsize_(bufsize)
        {
            init();
        }

        // copy
        buffer(const buffer &buf) = delete;
        buffer &operator=(const buffer &buf) = delete;

        // move
        buffer(buffer &&buf)
        : head_(buf.head_), buf_(buf.buf_), size_(buf.size_), bufsize_(buf.bufsize_)
        {
            buf.head_ = nullptr;
            buf.buf_ = nullptr;
        }

        buffer &operator=(buffer &&buf)
        {
            head_ = buf.head_;
            buf.head_ = nullptr;

            buf_ = buf.buf_;
            buf.buf_ = nullptr;

            size_ = buf.size_;
            bufsize_ = buf.bufsize_;
            return *this;
        }

        ~buffer()
        {
            internal_buffer::destroy(head_);
        }

        void push_back(T &&elem)
        {
            if(!buf_->push_back(std::move(elem)))
            {
                internal_buffer *n = internal_buffer::create(bufsize_);
                buf_->setNext(n);
                buf_ = n;

                if(!buf_->push_back(std::move(elem)))
                {
                    abort();
                }
            }

            size_++;
        }

        T &front() { return head_->front(); }
        const T &front() const { return head_->front(); }
        T &back() { return buf_->back(); }
        const T &back() const { return buf_->back(); }
        size_t size() const { return size_; }
        bool empty() const { return size_ == 0; }
    };
}

#endif
