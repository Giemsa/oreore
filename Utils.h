#ifndef __UTILS_H__
#define __UTILS_H__

/*
 * 雑多な便利ルーチン群
 */

#if __cplusplus > 199711L
#   define __IS_CPP_11__
#   define __constexpr constexpr
#else
#   define __constexpr
#endif

#ifdef USE_COCOS2DX
#   define __IS_COCOS2DX_ENABLED__
#   include "./cocos2d/Utils.h"
#endif

#include <stdlib.h>
#include <cstdarg>
#include <cmath>
#include <limits>

#include "TimeCounter.h"


namespace Utils
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

    struct Types
    {
#ifdef __IS_COCOS2DX_ENABLED__
        typedef GLubyte byte;
#else
        typedef unsigned char byte;
        
        struct Color3B
        {
            byte r, g, b;

            Color3B(): r(0), g(0), b(0) {}
            Color3B(const byte _r, const byte _g, const byte _b) : r(_r), g(_g), b(_b) { }
        };
#endif
    };
    
    
#ifndef __IS_COCOS2DX_ENABLED__
#   define Color3B Types::Color3B
#endif

    /* 色Aから色Bへのグラデーションを計算する(等比グラデーション) */
	template<
		int Ra, int Ga, int Ba,
		int Rb, int Gb, int Bb
	>
	inline Color3B gradient(const double count)
	{
		return Color3B(
			static_cast<Types::byte>((Rb - Ra) * count / 100 + Ra),
			static_cast<Types::byte>((Gb - Ga) * count / 100 + Ga),
			static_cast<Types::byte>((Ba - Ba) * count / 100 + Ba)
		);
	}
    
	inline Color3B gradient(const double count, const Color3B &a, const Color3B &b)
	{
		return Color3B(
			static_cast<Types::byte>((b.r - a.r) * count / 100 + a.r),
			static_cast<Types::byte>((b.g - a.g) * count / 100 + a.g),
			static_cast<Types::byte>((b.b - a.b) * count / 100 + a.b)
		);
	}


#ifdef __IS_CPP_11__
    template<typename T>
    constexpr T digits(const T n) { return static_cast<int>(std::ceil(log10(static_cast<long double>(n)))); }

    template<typename T>
    constexpr int digits() { return std::numeric_limits<T>::digits10 + 1; }
    
    template<int N>
    constexpr int digits() { return digits(N); }
#else
    template<typename T>
    inline T digits(const T n) { return static_cast<int>(std::ceil(log10(static_cast<long double>(n)))); }

    template<typename T>
    inline int digits() { return std::numeric_limits<T>::digits10 + 1; }

    template<int N>
    inline int digits() { return digits(N); }
#endif

    template<typename T>
    inline int sgn(const T v) { return ((static_cast<int>(v) >> std::numeric_limits<int>::digits) << 1) + 1; }

    template<typename T>
    inline T rad2deg(const T rad) { return rad * 180 / M_PI; }

    template<typename T>
    inline T deg2rad(const T deg) { return deg * M_PI / 180; }
}

#ifndef va_copy
#	define va_copy(dest, src) ((dest) = (src))
#endif

#undef __IS_COCOS2DX_ENABLED__
#undef __IS_CPP_11__
#undef __constexpr
#undef Color3B

#endif
