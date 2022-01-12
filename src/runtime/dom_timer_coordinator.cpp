//
// Created by didi on 2022/1/6.
//
#include <memory>
#include "vodka/runtime/dom_timer_coordinator.h"
#include "vodka/runtime/dom_timer.h"
#include "vodka/runtime/context.h"

namespace vodka {

int DOMTimerCoordinator::InstallNewTimeout(JSContext *context, std::function<void()> &&func, uint64_t delay_in_millisecond, bool single_shot) {

  int timeout_id = NextId();
  timers_[timeout_id] = std::make_unique<DOMTimer>(context, std::move(func), delay_in_millisecond, single_shot);;
  return timeout_id;
}
void DOMTimerCoordinator::RemoveTimeoutByID(int timeout_id) {

  if (timeout_id <= 0){return;}
  if (timers_.count(timeout_id)>0){
    timers_.erase(timeout_id);
  }
}

int DOMTimerCoordinator::NextId() {
  while (true) {
    ++circular_sequential_id_;
    if (circular_sequential_id_ <= 0){
      circular_sequential_id_ = 1;
    }
    if (timers_.count(circular_sequential_id_)<1){
      return circular_sequential_id_;
    }
  }
}
}
