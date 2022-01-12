//
// Created by didi on 2021/12/31.
//

#ifndef VODKA_CONTEXT_H
#define VODKA_CONTEXT_H

#pragma once

#include <memory>
#include <queue>

#include "vodka/js_task_runner.h"
#include "vodka/task.h"
#include "dom_timer_coordinator.h"

namespace vodka {
/**
 *
 * 微任务：https://html.spec.whatwg.org/#perform-a-microtask-checkpoint
 * 由于 JSC/Quick JS 本身维护了 micro task queue。因此只在 hermes engine 下可以选择启用 vodka 微任务支持。
 * 所有 api 必须在 JS 线程访问
 * */
class JSContext {
 public:

  static std::shared_ptr<JSContext> Create(const std::shared_ptr<JSTaskRunner> &task_runner);
  explicit JSContext(const std::shared_ptr<JSTaskRunner> &task_runner);
  int SetTimeout(std::function<void()> &&func, int timeout);
  int SetInterval(std::function<void()> &&func, int timeout);

  void ClearTimeout(int timeout_id);
  void ClearInterval(int timeout_id);

  inline JSTaskRunner *taskRunner() const {
      return task_runner_.get();
  };
  inline DOMTimerCoordinator *timers(){
    return &timers_;
  }

  JSContext(const JSContext &) = delete;
  JSContext& operator=(const JSContext &) = delete;
 private:
  DOMTimerCoordinator timers_;
  std::queue<std::shared_ptr<Task>> micro_task_;
  std::shared_ptr<JSTaskRunner> task_runner_;

};
}
#endif  // VODKA_CONTEXT_H
