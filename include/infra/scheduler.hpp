#pragma once

#include <functional>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    // Start the scheduler thread
    void start();

    // Stop the scheduler thread gracefully
    void stop();

    // Schedule a recurring task with interval in milliseconds
    void schedule_task(const std::function<void()>& task, std::chrono::milliseconds interval);

private:
    void run();

    struct Task {
        std::function<void()> func;
        std::chrono::milliseconds interval;
        std::chrono::steady_clock::time_point next_run;
    };

    std::vector<Task> tasks_;
    std::thread scheduler_thread_;
    std::mutex tasks_mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_;
};
