#ifndef __OREORE_DLOG_H__
#define __OREORE_DLOG_H__

#include <iostream>

namespace oreore
{
    class DLog final
    {
    public:
        enum class LogType : int
        {
            Info     = 0x01,
            Debug    = 0x02,
            Warning  = 0x04,
            Error    = 0x08,
            All      = Info | Debug | Warning | Error
        };

    private:
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
        bool showTime;

        DLog(const DLog &rhs) { }
        DLog(DLog &&rhs) { }
        DLog() : os(&std::cout), dfmt(os->flags()), showTime(false) { }
        ~DLog() { }

        void setFlags();
        void resetFlags();
        void parseArg(const char *&str);
        void outputTime();

        void output(const char *s);

        template<typename T, typename ...Args>
        void output(const char *s, const T &v, const Args&... args);

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
        inline static void out(const LogType type, const char *format, const Args&... args)
        {
            getInstance().outputTime();
            getInstance().output(format, args...);
        }

        template<typename ...Args>
        inline static void info(const char *format, const Args&... args)
        {
            out(LogType::Info, format, args...);
        }

        template<typename ...Args>
        inline static void debug(const char *format, const Args&... args)
        {
            out(LogType::Debug, format, args...);
        }

        template<typename ...Args>
        inline static void warn(const char *format, const Args&... args)
        {
            out(LogType::Warning, format, args...);
        }

        template<typename ...Args>
        inline static void error(const char *format, const Args&... args)
        {
            out(LogType::Error, format, args...);
        }

        template<typename T>
        inline static void outputFormat(const T &v)
        {
            return getInstance()._outputFormat(v);
        }

        inline static void initFlags() { getInstance().resetFlags(); }
        inline static void setShowTime(const bool st) { getInstance().showTime = st; }
        inline static bool isShowTime() { return getInstance().showTime; }
    };

    template<typename T, typename ...Args>
    void DLog::output(const char *s, const T &v, const Args&... args)
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
                    return output(++s, args...);
                case '#':
                    (*os) << v;
                    return output(++s, args...);
            }

            (*os) << *s++;
        }
        throw std::runtime_error("parameters mismatch.");
    }

    // shortcut
    template<typename ...Args>
    inline void dlog(const char *format, const Args&... args)
    {
        DLog::debug(format, args...);
    }
}

#endif
