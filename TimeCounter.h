#ifndef __OREORE_TIME_COUNTER_H__
#define __OREORE_TIME_COUNTER_H__

// 時間を測るクラス
// 必要になったらプラットフォームごとに精度高い関数使う
#include <ctime>
#include "null.h"

namespace oreore
{
    enum class TimerPrecision
    {
        Second,
        MilliSecond
    };
    
    namespace Times
    {
        template<TimerPrecision prec>
        struct Funcs
        {
            typedef time_t type;
            typedef time_t span_t;
            inline static type time() { return std::time(null); }
            inline static int getClockPerMSec() { return 1; }
        };
        

        template<>
        struct Funcs<TimerPrecision::MilliSecond>
        {
            typedef clock_t type;
            typedef double span_t;
            inline type static time() { return std::clock(); }
            inline int static getClockPerMSec() { return CLOCKS_PER_SEC / 1000; }
        };
    }

    template<TimerPrecision prec>
    class TimeCounter
    {
        typedef typename Times::Funcs<prec>::type TimeType;
        typedef typename Times::Funcs<prec>::span_t SpanType;
    private:
        TimeType begin;
        SpanType total;
    public:
        TimeCounter() : total(), begin() { }
    
        inline void start()
        {
            begin = Times::Funcs<prec>::time();
        }

        inline void stop()
        {
            total += static_cast<SpanType>((Times::Funcs<prec>::time() - begin)) / Times::Funcs<prec>::getClockPerMSec();
        }
        
        inline SpanType getTime() const { return total; }
        inline void reset() { total = SpanType(); }
    };
}

#endif
