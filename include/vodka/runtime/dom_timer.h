//
// Created by didi on 2021/12/29.
//

#ifndef VODKA_DOM_TIMER_H
#define VODKA_DOM_TIMER_H

#pragma once
#include "timer.h"

namespace vodka {
class JSContext;
class DOMTimer : public TimerBase{
 public:

  explicit DOMTimer(JSContext *context, std::function<void()> &&func, uint64_t delay_in_millisecond, bool single_shot);

  void Fired() override;
  void set_timeout_id(int timeout_id){
    timeout_id_ = timeout_id;
  };

 private:
  int timeout_id_;
  std::function<void()> callback_;
};

}
#endif  // VODKA_DOM_TIMER_H
