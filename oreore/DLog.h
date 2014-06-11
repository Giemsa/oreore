#ifndef __OREORE_DLOG_H__
#define __OREORE_DLOG_H__

#include <iostream>

namespace oreore
{
    class DLog final
    {
        enum class TextAlign
        {
            Left,
            Center,
            Right
        };

        enum class FormatType
        {
            _INTEGER,
            _FLOAT,
            _BOOL,
            _STRING,
            _HEX,
            _HEX_U,
            _OCT,
            AUTO
        };

        struct PrintFormat
        {
            TextAlign align;
            FormatType format;
            char fillChar;
            int len;
            bool showpos;

            PrintFormat()
                : align(TextAlign::Left), format(FormatType::AUTO),
                  fillChar(' '), len(0), showpos(false)
                { }
        };
    private:
        std::ostream *os;
        PrintFormat currentFormat;
        std::ios::fmtflags dfmt;

        DLog(const DLog &rhs) { }
        DLog(DLog &&rhs) { }
        DLog() : os(&std::cout), dfmt(os->flags()) { }
        ~DLog() { }

        void setFlags();
        void resetFlags();
        void parseArg(const char *&str);

        void dlog(const char *s);

        template<typename T, typename ...Args>
        void dlog(const char *s, const T &v, const Args&... args);

        template<typename T>
        inline void _outputFormat(const T &v)
        {
            setFlags();
            (*os) << v;
            resetFlags();
        }

    public:
        inline static DLog &getInstance()
        {
            static DLog log;
            return log;
        }

        inline static void registration(std::ostream &stream)
        {
            getInstance().os = &stream;
            getInstance().dfmt = stream.flags();
        }

        template<typename ...Args>
        inline static void out(const char *format, const Args&... args)
        {
            getInstance().dlog(format, args...);
        }


        template<typename T>
        inline static void outputFormat(const T &v)
        {
            return getInstance()._outputFormat(v);
        }

        inline static void initFlags() { getInstance().resetFlags(); }
    };

    template<typename T, typename ...Args>
    void DLog::dlog(const char *s, const T &v, const Args&... args)
    {
        while(*s)
        {
            switch(*s)
            {
                case '\\':
                    ++s;
                    break;
                case '{':
                    parseArg(s);
                    outputFormat(v);
                    return dlog(++s, args...);
                case '#':
                    (*os) << v;
                    return dlog(++s, args...);
            }

            (*os) << *s++;
        }
        throw std::runtime_error("parameters mismatch.");
    }
}

#endif
