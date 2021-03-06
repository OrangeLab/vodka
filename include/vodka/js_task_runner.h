//
// Created by didi on 2021/12/24.
//

#ifndef VODKA_JS_TASK_RUNNER_H
#define VODKA_JS_TASK_RUNNER_H

#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "task.h"
#include "task_runner.h"
#include "thread.h"
#include "time.h"

namespace vodka {
class JSContext;
class JSTask : public Task {
 public:
  using Function = std::function<void()>;
  // context 默认为 nullptr, 只在 context 为 micro task 时有效。
  explicit JSTask(Function&& func, const std::shared_ptr<JSContext>& context = nullptr);

  void Run() override;
  inline void Cancel() { canceled_ = true; }

 private:
  std::weak_ptr<JSContext> context_;
  Function callback_ = nullptr;
};

/**
 * 实现标准：
 * 1. 当 TaskRunner 自身被释放后，内部任务不在被执行。
 *
 * */
class JSTaskRunner : public TaskRunner {
 public:
  JSTaskRunner();
  ~JSTaskRunner() override;
  JSTaskRunner(const JSTaskRunner&) = delete;
  JSTaskRunner& operator=(const JSTaskRunner&) = delete;

  void PostTask(std::shared_ptr<Task> task) override;

  // 只能在 JS 线程访问
  void PostDelayedTask(std::shared_ptr<Task> task,
                       int64_t delay_in_milliseconds) override;
  void Quit() override;

  // test only
  inline TimeDelta GetNextWakeUp() const {
      return next_wake_up_time_;
  };
 private:
  class JSThread : public Thread {
   public:
    explicit JSThread(JSTaskRunner* taskRunner);
    void Run() override;

    JSThread(const JSThread&) = delete;
    JSThread& operator=(const JSThread&) = delete;

    std::atomic<bool> no_runner{false};
  private:
    JSTaskRunner* taskRunner_;
  };

  using DelayedPair = std::pair<TimeDelta, std::shared_ptr<Task>>;
  struct CompareEntry {
    bool operator()(const DelayedPair& __x, const DelayedPair& __y) const {
      return __x.first.stamp() > __y.first.stamp();
    }
  };
  std::shared_ptr<Task> GetNext();

  std::atomic<bool> is_quit_{false};
  std::mutex mutex_;
  std::condition_variable cv_;
  std::queue<std::shared_ptr<Task>> task_queue_;
  std::unique_ptr<JSThread> thread_;
  std::priority_queue<DelayedPair, std::vector<DelayedPair>, CompareEntry>
      delayed_task_queue_;

  TimeDelta next_wake_up_time_{0};
};

class GlobalJSTaskRunner {
 public:
  /// it's not thread-safe. must be called at platform main thread
  static std::shared_ptr<JSTaskRunner> GetInstance();

 private:
  static std::weak_ptr<JSTaskRunner> reuse_instance_;
};

}  // namespace vodka

#endif  // VODKA_JS_TASK_RUNNER_H
