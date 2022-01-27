//
// Created by didi on 2021/12/29.
//

#include "vodka/runtime/dom_timer.h"

#include "vodka/runtime/context.h"

namespace vodka {

DOMTimer::DOMTimer(JSContext *context, std::function<void()> &&func,
                   uint64_t delay_in_millisecond, bool single_shot)
    : TimerBase(context->taskRunner()),
      callback_(std::move(func)),
      context_(context) {
  if (single_shot){
    StartOneShot(delay_in_millisecond);
  }else{
    StartRepeating(delay_in_millisecond);
  }
}

void DOMTimer::Fired() {
  if (callback_) {
    callback_();
  }
  if (single_shot()) {
    context_->timers()->RemoveTimeoutByID(timeout_id_);
  }
}
}  // namespace vodka