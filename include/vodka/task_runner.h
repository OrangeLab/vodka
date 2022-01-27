//
// Created by didi on 2021/12/24.
//

#ifndef VODKA_TASK_RUNNER_H
#define VODKA_TASK_RUNNER_H

#pragma once
#include <memory>

namespace vodka{

class Task;
class TaskRunner {
 public:
  virtual ~TaskRunner() {};
  virtual void PostTask(std::shared_ptr<Task> task) = 0;
  virtual void PostDelayedTask(std::shared_ptr<Task> task, int64_t delay_in_milliseconds) = 0;
  virtual void Quit() = 0;
};
}


#endif  // VODKA_TASK_RUNNER_H
