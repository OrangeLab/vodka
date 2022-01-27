//
// Created by didi on 2022/1/27.
//
#include <gtest/gtest.h>
#include "vodka/runtime/context.h"
#include "vodka/semaphore.h"

// api
TEST(JSContextTest, timeout) {

  std::vector<int> order;
  vodka::Semaphore semaphore(0);

  std::shared_ptr<vodka::JSTaskRunner> runner = vodka::GlobalJSTaskRunner::GetInstance();
  std::shared_ptr<vodka::JSContext> context = std::make_shared<vodka::JSContext>(runner);
  context->SetTimeout([&]{
    order.push_back(0);
    semaphore.Signal();
  },1000);

  bool signalled = semaphore.WaitFor(1500);
  ASSERT_TRUE(signalled);
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(0, order[0]);

  int tid = context->SetTimeout([&]{
    order.push_back(1);
    semaphore.Signal();
  },1000);
  context->ClearTimeout(tid);
  signalled = semaphore.WaitFor(1500);
  ASSERT_FALSE(signalled);
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(0, order[0]);
};


TEST(JSContextTest, timeInterval) {

  std::vector<int> order;
  vodka::Semaphore semaphore(0);

  std::shared_ptr<vodka::JSTaskRunner> runner = vodka::GlobalJSTaskRunner::GetInstance();
  std::shared_ptr<vodka::JSContext> context = std::make_shared<vodka::JSContext>(runner);
  int count = 1;
  int tid = context->SetInterval([&]{
    order.push_back(0);
    if (count == 2){
      semaphore.Signal();
    }
    count++;
  },1000);

  bool signalled = semaphore.WaitFor(2500);
  if (signalled) {
    context->ClearInterval(tid);
  }
  ASSERT_TRUE(signalled);
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(0, order[0]);
  ASSERT_EQ(0, order[0]);

  int tid2 = context->SetInterval([&]{
    order.push_back(1);
    semaphore.Signal();
  },1000);

  context->ClearInterval(tid2);
  signalled = semaphore.WaitFor(1500);
  ASSERT_FALSE(signalled);
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(0, order[0]);
  ASSERT_EQ(0, order[0]);
}
