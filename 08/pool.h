#pragma once

#include <thread>
#include <future>
#include <atomic>
#include <condition_variable>
#include <functional>

#include <queue>


template<class T>
struct void_check {};
template<>
struct void_check<void>
{ typedef void type; };

template<class T>
struct nonvoid_check
{ typedef T type; };
template<>
struct nonvoid_check<void> {};


class ThreadPool
{
    size_t poolSize_;
    bool active_;

    std::queue<std::function<void ()> > queue_;
    std::queue<std::thread> pool_;

    std::mutex mutex_;
    std::condition_variable wakeUp_;

public:
    explicit ThreadPool(size_t poolSize)
        : poolSize_ (poolSize)
        , active_(true)
    {
        for (size_t i = 0; i < poolSize_; i++)
        {
            pool_.push(std::thread([this]() {
                while (true)
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    if (!queue_.empty())
                    {
                        auto func = queue_.front();
                        queue_.pop();
                        lock.unlock();

                        func();
                    }
                    else
                    {
                        if (active_) wakeUp_.wait(lock);
                        else break;
                    }
                }
            }));
        }
    }

    ~ThreadPool ()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            active_ = false;
        }
        wakeUp_.notify_all();
        while (!pool_.empty())
        {
            pool_.front().join();
            pool_.pop();
        }
    }


    template <class Func, class... Args>
    auto exec(Func func, Args... args
    ) -> std::future<typename void_check<decltype(func(args...))>::type>
    {
        auto p = std::make_shared<std::promise<void> >();

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push([=]() mutable {
                func(args...);
                p->set_value();
            });
        }
        wakeUp_.notify_one();

        return p->get_future();
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args
    ) -> std::future<typename nonvoid_check<decltype(func(args...))>::type>
    {
        using output_type = decltype(func(args...));
        auto p = std::make_shared<std::promise<output_type> >();

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push([=]() mutable {
                p->set_value(func(args...));
            });
        }
        wakeUp_.notify_one();

        return p->get_future();
    }
};
