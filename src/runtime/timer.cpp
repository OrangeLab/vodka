//
// Created by didi on 2021/12/29.
//

#include <memory>
#include "vodka/runtime/timer.h"
#include "vodka/js_task_runner.h"
#include "vodka/runtime/context.h"
namespace vodka {


TimerBase::TimerBase(JSContext *context, bool single_shot) : context_(context), single_shot_(single_shot){ }

TimerBase::~TimerBase() {
  Stop();
}

void TimerBase::Start(uint64_t delay_in_millisecond) {
  delay_in_millisecond_ = delay_in_millisecond;
  SetNextFireTime();
}

void TimerBase::RunInternal() {
  if (!single_shot_){
    SetNextFireTime();
  }
  Fired();
}

void TimerBase::SetNextFireTime() {
  timeout_task_ = std::make_shared<JSTask>(std::bind(&TimerBase::RunInternal, this));
  context_->taskRunner()->PostDelayedTask(timeout_task_, delay_in_millisecond_);
}

void TimerBase::Stop() {
  if (this->timeout_task_){
    this->timeout_task_->Cancel();
  }
}
}
