//
// Created by didi on 2022/1/5.
//

#include "vodka/runtime/context.h"

namespace vodka {

JSContext::JSContext(const std::shared_ptr<JSTaskRunner> &task_runner):task_runner_(task_runner){ };

std::shared_ptr<JSContext> JSContext::Create(const std::shared_ptr<JSTaskRunner> &task_runner) {
  return std::make_shared<JSContext>(task_runner);
}

int JSContext::SetInterval(std::function<void()> &&func, int timeout) {
  return timers_.InstallNewTimeout(this, std::move(func), timeout, false);
}
int JSContext::SetTimeout(std::function<void()> &&func, int timeout) {
  return timers_.InstallNewTimeout(this, std::move(func), timeout, true);
}
void JSContext::ClearInterval(int timeout_id) {

  timers_.RemoveTimeoutByID(timeout_id);
}

void JSContext::ClearTimeout(int timeout_id) {

  timers_.RemoveTimeoutByID(timeout_id);
}
}
