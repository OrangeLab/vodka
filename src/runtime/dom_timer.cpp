//
// Created by didi on 2021/12/29.
//

#include "vodka/runtime/dom_timer.h"
#include "vodka/runtime/context.h"

namespace vodka {

DOMTimer::DOMTimer(JSContext *context, std::function<void()> &&func, uint64_t delay_in_millisecond, bool single_shot) : TimerBase(context ,single_shot), callback_(std::move(func)) {
  Start(delay_in_millisecond);
}

void DOMTimer::Fired() {
  if (callback_){
    callback_();
  }
  if (single_shot()){
    context()->timers()->RemoveTimeoutByID(timeout_id_);
  }
}
}