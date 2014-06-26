#ifndef __NULL_H__
#define __NULL_H__

#if __cplusplus > 199711L
const decltype(nullptr) null = nullptr;
#else

const class null_t {
public:
    template<class T>
    operator T*() const { return 0; }
    template<class C, class T>
    operator T C::*() const { return 0; }
private:
    void operator&() const;
} null = {};

#endif

#endif

