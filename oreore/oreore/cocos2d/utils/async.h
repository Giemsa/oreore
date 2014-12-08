#ifndef __OREORE_UTILS_ASYNC_H__
#define __OREORE_UTILS_ASYNC_H__

#include <functional>
#include <atomic>
#include <thread>
#include "cocos2d.h"

namespace oreore
{
    /**
     * 単純な非同期処理を行うクラス
     * 登録された処理は別スレッドで並列実行されます。
     * 登録されたコールバックはメインスレッドで実行されます。
     */
    class AsyncTask final
    {
        using Callback = std::function<void(bool)>;

        template<typename... Args>
        class Task final
        {
            using TaskType = std::function<bool(Args...)>;
            using BindedFunc = decltype(
                    std::bind(
                        std::declval<TaskType>(),
                        std::declval<Args>()...
                    )
                );

        private:
            std::atomic<int> end;
            bool result;
            BindedFunc task;
            Callback callback;

        public:
            Task(const TaskType &task, const Callback &callback, Args&&... args)
            : end(0), result(false)
            , callback(callback), task(task, std::forward<Args>(args)...)
            { }

            void update(const float dt)
            {
                if(end.load() == 1)
                {
                    if(callback)
                    {
                        callback(result);
                    }

                    cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
                    delete this;
                }
            }

            inline void run()
            {
                result = task();
                end.store(1);
            }

            ~Task() = default;
        };

    private:
        AsyncTask(const AsyncTask &) = delete;
        AsyncTask &operator=(const AsyncTask &) = delete;

    public:
        template<typename F, typename... Args>
        AsyncTask(const F &func, const Callback &callback, Args&&... args)
        {
            Task<Args...> *task = new Task<Args...>(func, callback, std::forward<Args>(args)...);
            cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(task, 1024, false);

            std::thread th([task]() {
                task->run();
            });

            th.detach();
        }

        ~AsyncTask() = default;
    };
}


#endif
