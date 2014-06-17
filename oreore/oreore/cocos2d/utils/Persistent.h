#ifndef __OREORE_COCOS2D_PERSISTENT_H__
#define __OREORE_COCOS2D_PERSISTENT_H__

#include "cocos2d.h"

namespace oreore
{
    template<typename T>
    class Persistent final
    {
        using self_type = Persistent<T>;
    private:
        T *ptr;

        inline void retain()
        {
            if(ptr)
            {
                ptr->retain();
            }
        }

        inline void release()
        {
            if(ptr)
            {
                ptr->release();
            }
        }
    public:
        inline Persistent() : ptr(nullptr) { }

        inline Persistent(const Persistent &rhs)
        {
            rhs.ptr->retain();
            ptr = rhs.ptr;
        }

        inline Persistent(Persistent &&rhs)
        {
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
        }

        inline Persistent(T *p) : ptr(p)
        {
            retain();
        }

        inline ~Persistent()
        {
            if(ptr)
            {
                ptr->release();
                ptr = nullptr;
            }
        }

        inline T *operator->() const
        {
            return ptr;
        }

        inline operator bool() const
        {
            return ptr != nullptr;
        }

        inline bool operator!() const
        {
            return !operator bool();
        }

        inline operator T *() const
        {
            return *ptr;
        }

        inline bool operator==(T *p) const
        {
            return p == ptr;
        }

        inline bool operator==(const self_type &rhs) const
        {
            return rhs.ptr == ptr;
        }

        inline bool operator!=(T *p) const
        {
            return !operator==(p);
        }

        inline bool operator!=(const self_type &rhs) const
        {
            return !operator==(rhs);
        }

        self_type &operator=(T *p)
        {
            release();
            ptr = p;
            retain();

            return *this;
        }

        self_type &operator=(const self_type &rhs)
        {
            release();
            ptr = rhs.ptr;
            retain();
            return *this;
        }

        self_type &operator=(self_type &&rhs)
        {
            release();
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
            return *this;
        }

        inline T *get() const { return ptr; }
    };

    template<typename T>
    Persistent<T> persist(T *ptr)
    {
        return Persistent<T>(ptr);
    }
}

#endif
