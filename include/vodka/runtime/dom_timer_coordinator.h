//
// Created by didi on 2021/12/31.
//

#ifndef VODKA_DOM_TIMER_COORDINATOR_H
#define VODKA_DOM_TIMER_COORDINATOR_H

#pragma once

#include <map>
#include <memory>
#include "dom_timer.h"

namespace vodka {

class JSContext;
class DOMTimerCoordinator {
 public:
  DOMTimerCoordinator() = default;
  DOMTimerCoordinator(const DOMTimerCoordinator&) = delete;
  DOMTimerCoordinator& operator=(const DOMTimerCoordinator&) = delete;

  ~DOMTimerCoordinator() = default;
  // Creates and installs a new timer. Returns the assigned ID.
  int InstallNewTimeout(JSContext *context,
                        std::function<void()> &&func,
                        uint64_t delay_in_millisecond,
                        bool single_shot);

  void RemoveTimeoutByID(int timeout_id);

 private:

  int NextId();

  int circular_sequential_id_ = 0;
  std::map<int, std::unique_ptr<DOMTimer>> timers_;
};

}
#endif  // VODKA_DOM_TIMER_COORDINATOR_H
