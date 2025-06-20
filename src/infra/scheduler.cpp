#include "../include/infra/scheduler.hpp"
#include "../include/utils/logger.hpp"

Scheduler::Scheduler()
    : running_(false)
{}

Scheduler::~Scheduler() {
    stop();
}

void Scheduler::start() {
    if (running_) return;

    running_ = true;
    scheduler_thread_ = std::thread(&Scheduler::run, this);

    auto logger = Logger::get_instance();
    logger->info(" Scheduler started.");
}

void Scheduler::stop() {
    if (!running_) return;

    running_ = false;
    cv_.notify_all();

    if (scheduler_thread_.joinable()) {
        scheduler_thread_.join();
    }

    auto logger = Logger::get_instance();
    logger->info(" Scheduler stopped.");
}

void Scheduler::schedule_task(const std::function<void()>& task, std::chrono::milliseconds interval) {
    std::lock_guard<std::mutex> lock(tasks_mutex_);
    tasks_.push_back({task, interval, std::chrono::steady_clock::now() + interval});
}

void Scheduler::run() {
    while (running_) {
        std::unique_lock<std::mutex> lock(tasks_mutex_);

        if (tasks_.empty()) {
            cv_.wait(lock, [this]{ return !running_ || !tasks_.empty(); });
        } else {
            auto now = std::chrono::steady_clock::now();
            for (auto& task : tasks_) {
                if (now >= task.next_run) {
                    // Run task without holding lock (to avoid deadlocks if task schedules more)
                    lock.unlock();
                    task.func();
                    lock.lock();

                    // Schedule next run
                    task.next_run = now + task.interval;
                }
            }

            // Wait for the shortest time until next task is due, or wakeup for stop
            auto next_due = tasks_.front().next_run;
            for (const auto& task : tasks_) {
                if (task.next_run < next_due) {
                    next_due = task.next_run;
                }
            }

            cv_.wait_until(lock, next_due, [this]{ return !running_; });
        }
    }
}
