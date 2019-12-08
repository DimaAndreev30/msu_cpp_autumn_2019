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
    std::atomic<bool> active_;

    std::queue<std::function<void ()> > queue_;
    std::queue<std::thread> pool_;

    std::mutex mutex_;
    std::condition_variable wakeUp_;


    template<class T, class... Args>
    void setoutput(std::shared_ptr<std::promise<T> >& p,
                   std::function<T (Args...)> func, Args&... args,
                   typename nonvoid_check<T>::type* =nullptr)
    {
        p->set_value(func(args...));
    }
    template<class T, class... Args>
    void setoutput(std::shared_ptr<std::promise<T> >& p,
                   std::function<T (Args...)> func, Args&... args,
                   typename void_check<T>::type* =nullptr)
    {
        func(args...);
        p->set_value();
    }

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
        active_ = false;
        wakeUp_.notify_all();
        while (!pool_.empty())
        {
            pool_.front().join();
            pool_.pop();
        }
    }


    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        using output_type = decltype(func(args...));
        auto p = std::make_shared<std::promise<output_type> >();

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push([=]() mutable {
                setoutput<output_type, Args...>(p, func, args...);
            });
        }
        wakeUp_.notify_one();

        return p->get_future();
    }
};
