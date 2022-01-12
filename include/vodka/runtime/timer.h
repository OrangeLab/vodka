//
// Created by didi on 2021/12/29.
//

#ifndef VODKA_TIMER_H
#define VODKA_TIMER_H
#pragma once

#include <functional>
#include <memory>
#include "vodka/js_task_runner.h"
namespace vodka {
class JSContext;
/**
 * (强)引用关系
 * context -> timer
 * timer -> task
 *
 * 异常：
 * context -> dangle ptr
 * 单线程及单持有(timer 只被context持有)的存在，保证了只要 timer 存在，则 context 的指针安全。
 *
 * std::function -> bind(timer 成员函数)
 * 由于 task 的生命周期可能长于 timer，因此存在 timer 析构后，仍被 task->callback 调用，从而导致 bad_function_call
 * 这里通过 timer 析构时，主动 cancel task 规避。由于 timer 存在与 JS 线程，因此直接访问 cancel 是安全的。
 *
 * */
class TimerBase {
 public:
  virtual ~TimerBase();

  explicit TimerBase(JSContext *context, bool single_shot);
  TimerBase(const TimerBase&) = delete;
  TimerBase& operator=(const TimerBase&) = delete;

  void Start(uint64_t delay_in_millisecond);
  virtual void Stop();
  virtual void Fired() {};

  inline JSContext *context(){
      return context_;
  };

  inline bool single_shot(){
    return single_shot_;
  };

 private:
  void RunInternal();
  void SetNextFireTime();

  std::shared_ptr<JSTask> timeout_task_;
  JSContext *context_;
  bool single_shot_;
  uint64_t delay_in_millisecond_;
};

}
#endif  // VODKA_TIMER_H
