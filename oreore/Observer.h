#ifndef __OREORE_OBSERVER_H__
#define __OREORE_OBSERVER_H__

#include <functional>
#include <cassert>
#include <unordered_map>
#include <utility>

namespace oreore
{
    class Observer final
    {
        class SubjectBase
        {
        public:
            virtual ~SubjectBase() { }
        };

        template<typename T>
        class Subject final : public SubjectBase
        {
        private:
            const std::function<T> _func;
        public:
            inline Subject(const std::function<T> &func) : _func(func) { }

            template<typename... A>
            inline void call(const A &...args) { _func(args...); }
        };

        using SubjectList = std::unordered_map<int, SubjectBase *>;
    private:
        SubjectList subjectList;

        inline Observer() { }
        inline ~Observer() { clear(); }
    public:
        inline static Observer &getInstance()
        {
            static Observer observer;
            return observer;
        }

        inline void clear()
        {
            for(const auto &e : subjectList)
                delete e.second;
            subjectList.clear();
        }

        template<
            typename T, typename E,
            typename = typename std::enable_if<std::is_enum<E>::value, void>::type
        >
        void addSubject(const E key, const std::function<T> &callback);

        template<
            typename E, typename ...Ts,
            typename = typename std::enable_if<std::is_enum<E>::value, void>::type
        >
        void notify(const E key, const Ts &...args);

        template<
            typename E,
            typename = typename std::enable_if<std::is_enum<E>::value, void>::type
        >
        void removeSubject(const E key);
    };


    template<typename T, typename E, typename>
    void Observer::addSubject(const E key, const std::function<T> &callback)
    {
        auto *f = new Subject<T>(callback);
        const auto result = subjectList.insert(std::make_pair(static_cast<int>(key), f));

        if(!result.second)
        {
            assert(!"key already exists");
            delete f;
        }
    }

    template<typename E, typename ...Ts, typename>
    void Observer::notify(const E key, const Ts &...args)
    {
        const auto it = subjectList.find(static_cast<int>(key));
        if(it == subjectList.end())
        {
            assert(!"key does not exist");
            return;
        }

        auto *f = dynamic_cast<Subject<void (Ts...)> *>(it->second);
        if(!f)
        {
            assert(!"type not match.");
            return;
        }

        f->call(args...);
    }


    template<typename E, typename>
    void Observer::removeSubject(const E key)
    {
        const auto it = subjectList.find(static_cast<int>(key));
        if(it != subjectList.end())
        {
            delete it->second;
            subjectList.erase(it);
        }
    }
}

#endif