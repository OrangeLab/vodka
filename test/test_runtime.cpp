//
// Created by didi on 2022/1/27.
//
#include <gtest/gtest.h>
#include "vodka/runtime/runtime.h"
#include "vodka/runtime/context.h"
#include "vodka/semaphore.h"
//std::shared_ptr<JSContext> context_;
//std::shared_ptr<JSTaskRunner> task_runner_;
//RuntimeConfig config_;
//std::atomic_bool is_destory_{false};

class TestRuntime : public vodka::Runtime {
 public:
  ~TestRuntime(){}
  std::shared_ptr<vodka::JSContext> context(){
    return context_;
  }

  std::shared_ptr<vodka::JSTaskRunner> task_runner(){
    return task_runner_;
  }
};
