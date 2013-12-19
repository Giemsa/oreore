#ifndef __OREORE_UTILS_H__
#define __OREORE_UTILS_H__

/*
 * 雑多な便利ルーチン群
 */

#if __cplusplus > 199711L
#   define __IS_CPP_11__
#   define __constexpr constexpr
#else
#   define __constexpr inline
#endif

#ifdef USE_COCOS2DX
#   include "./cocos2d/Utils.h"
#endif

#include <stdlib.h>
#include <cstdarg>
#include <cmath>
#include <limits>

#include "TimeCounter.h"


namespace oreore
{
    // use Xorshift
	class Random
	{
	private:
		unsigned long x, y, z, w;
	public:
		Random(const unsigned long seed) : x(123456789), y(362436069), z(seed), w(88675123)
		{
			w ^= seed;
			x ^= seed;
		}
		
		unsigned long next()
		{
			const unsigned long t = (x ^ (x << 11));
			x = y;
			y = z;
			z = w;
			return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
		}
		
		inline unsigned long next(const unsigned long max) { return next() % max; }
		inline unsigned long next(const unsigned long min, const unsigned long max) { return min + next(max - min); }
	};
	
	unsigned long random();
	inline unsigned long random(const unsigned long max) { return random() % max; }
	inline unsigned long random(const unsigned long min, const unsigned long max) { return min + random(max - min); }
	
	template<typename T>
	inline T random(const unsigned long max) { return static_cast<T>(random() % max); }
	
	template<typename T>
	inline T random(const unsigned long min, const unsigned long max) { return static_cast<T>(min + random(max - min)); }

    /* 範囲指定する */
    template<typename T>
    T range(const T min, const T value, const T max)
    {
        if(value < min)
            return min;
        if(value > max)
            return max;
        return value;
    }

    template<typename T>
    __constexpr T digits(const T n) { return static_cast<int>(std::ceil(log10(static_cast<long double>(n)))); }

    template<typename T>
    __constexpr int digits() { return std::numeric_limits<T>::digits10 + 1; }
    
    template<int N>
    __constexpr int digits() { return digits(N); }

    template<typename T>
    __constexpr int sgn(const T v) { return ((static_cast<int>(v) >> std::numeric_limits<int>::digits) << 1) + 1; }

    __constexpr double rad2deg(const double rad) { return rad * 180 / M_PI; }

    __constexpr double deg2rad(const double deg) { return deg * M_PI / 180; }
}

#ifndef va_copy
#	define va_copy(dest, src) ((dest) = (src))
#endif

#define property_rw(Type, Name, Field)  \
    private: \
        Type Field;   \
    public:     \
        inline Type get ## Name() const { return Field; } \
        inline void set ## Name(Type Field) { this->Field = Field; } \
    private:

#define property_r(Type, Name, Field)  \
    private: \
        Type Field;   \
    public:     \
        inline Type get ## Name() const { return Field; } \
    private:

#define property_w(Type, Name, Field)  \
    private: \
        Type Field;   \
    public:     \
        inline void set ## Name(Type Field) { this->Field = Field; } \
    private:

#define property(Attr, Type, Name, Field)  property_##Attr(Type, Name, Field)

#undef __IS_CPP_11__
#undef __constexpr

#endif
