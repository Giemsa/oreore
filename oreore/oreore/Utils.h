#ifndef __OREORE_UTILS_H__
#define __OREORE_UTILS_H__

/*
 * 雑多な便利ルーチン群
 */

#include <cmath>
#include <limits>
#include <random>

namespace oreore
{
    template<unsigned int N, typename T>
    constexpr T rotl(const T v)
    {
        return (v << N) | (v >> (sizeof(T) * std::numeric_limits<char>::digits - N));
    }

    class xorshift
    {
    public:
        using result_type = unsigned int;
    private:
        result_type x, y, z, w;

        static constexpr result_type default_seed = 541238617;
    public:
        xorshift() : xorshift(default_seed) { }
        explicit xorshift(const result_type seed)
            : x(123456789), y(362436069), z(521288629), w(88675123)
        {
            x ^= seed;
            y ^= rotl<8>(seed);
            z ^= rotl<16>(seed);
            w ^= rotl<24>(seed);
        }

        static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
        static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
        result_type operator()()
        {
            const result_type t = (x ^ (x << 11));
            x = y;
            y = z;
            z = w;
            return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
        }
    };

    /*
     * Random Number Generator using Xorshift
     * You can use this class for std::random_shuffle()
     */
    class Random
    {
        using result_type = unsigned long;
        using dist_t = std::uniform_int_distribution<result_type>;
    private:
        xorshift engine;
        dist_t dist;
    public:
        Random(const result_type seed) : engine(seed)
        { }

        inline result_type next()
        {
            return engine();
        }

        inline result_type next(const result_type max)
        {
            return dist(engine, dist_t::param_type(0, max));
        }

        inline result_type next(const result_type min, const result_type max)
        {
            return dist(engine, dist_t::param_type(min, max));
        }

        inline result_type operator()(const result_type max) { return next(max); }
    };

    unsigned long random();
    unsigned long random(const unsigned long max);
    unsigned long random(const unsigned long min, const unsigned long max);

    template<typename T>
    inline T random(const unsigned long max) { return static_cast<T>(random(max)); }

    template<typename T>
    inline T random(const unsigned long min, const unsigned long max) { return static_cast<T>(random(min, max)); }

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
    constexpr T _digits(const T n)
    {
        return n > 0 ? _digits<T>(n / 10) + 1 : 0;
    }

    template<typename T>
    constexpr T digits(const T n) { return _digits<T>(n / 10) + 1;  }

    template<typename T>
    constexpr int digits() { return std::numeric_limits<T>::digits10 + 1; }

    constexpr int sgn(const int v) { return ((v >> std::numeric_limits<int>::digits) << 1) + 1; }
    constexpr int sgn(const double v) { return (v > 0) - (v < 0); }
    constexpr int sgn(const float v) { return (v > 0) - (v < 0); }

    constexpr double rad2deg(const double rad) { return rad * 180 / M_PI; }

    constexpr double deg2rad(const double deg) { return deg * M_PI / 180; }
}

#ifndef va_copy
#   define va_copy(dest, src) ((dest) = (src))
#endif

#endif

