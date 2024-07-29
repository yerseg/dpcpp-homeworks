#include <functional>
#include <iostream>
#include <queue>
#include <thread>

class TaskScheduler {
public:
    using Task = std::function<void()>;

public:
    TaskScheduler()
        : thread_(&TaskScheduler::ThreadLoop, this) {
    }

    ~TaskScheduler() {
        running_ = false;
        thread_.join();
    }

public:
    void Schedule(Task task) {
        std::lock_guard lock(mutex_);
        tasks_.push(std::move(task));
    }

private:
    void ThreadLoop() {
        while (true) {
            std::queue<Task> tasks;
            {
                std::lock_guard lock(mutex_);
                std::swap(tasks, tasks_);
            }
            while (!tasks.empty()) {
                tasks.front()();
                tasks.pop();
            }
            if (!running_) {
                break;
            }
        }
    }

private:
    std::thread thread_;
    std::mutex mutex_;
    std::atomic_bool running_ = true;
    std::queue<std::function<void()>> tasks_;
};

int main() {
    {
        TaskScheduler scheduler;
        scheduler.Schedule([] { std::cout << "Hello, World!" << std::endl; });
        scheduler.Schedule([] {
            std::cout << "Hi, World!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
        scheduler.Schedule([] { std::cout << "Bonjour, World!" << std::endl; });
    }
    return 0;
}
