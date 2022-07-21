#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <future>


namespace MyDB{
    class ThreadPool {
    public:
        explicit ThreadPool(size_t threadNum) : stopThreadPool(false) {
            for(size_t i = 0; i < threadNum; ++i) {
                workerThreads.emplace_back([this]() {
                    for(;;) {
                        std::function<void()> task;  //change here!!!!!!!
                        {
                            std::unique_lock<std::mutex> ul(threadPoolLock);
                            threadPoolCV.wait(ul, [this]() { return stopThreadPool || !waitingTasks.empty(); });
                            if(stopThreadPool && waitingTasks.empty()) { return; }
                            task = std::move(waitingTasks.front());
                            waitingTasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> ul(threadPoolLock);
                stopThreadPool = true;
            }
            threadPoolCV.notify_all();
            for(auto& worker : workerThreads) {
                worker.join();
            }
        }

        template<typename F, typename... Args>
        auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
            auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );
            {
                std::unique_lock<std::mutex> ul(threadPoolLock);
                if(stopThreadPool) { throw std::runtime_error("ThreadPool shut down"); }
                waitingTasks.emplace([taskPtr]() { (*taskPtr)(); });
            }
            threadPoolCV.notify_one();
            return taskPtr->get_future();
        }

    private:
        bool stopThreadPool;
        std::vector<std::thread> workerThreads;
        std::queue<std::function<void()>> waitingTasks;
        std::mutex threadPoolLock;
        std::condition_variable threadPoolCV;
    };
}
#endif