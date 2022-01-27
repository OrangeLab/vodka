//
// Created by didi on 2022/1/19.
//
#include <gtest/gtest.h>
#include "vodka/js_task_runner.h"
#include "vodka/semaphore.h"

/**
 * task runner 线程安全性
 * */

class FakeClock {
 public:
  static double time() { return time_.load(); }
  static void set_time(double time) { time_.store(time); }
  static void set_time_and_wake_up_runner(
      double time, vodka::JSTaskRunner* runner) {
    time_.store(time);
    // PostTask will cause the condition variable WaitFor() call to be notified
    // early, rather than waiting for the real amount of time. WaitFor() listens
    // to the system clock and not our FakeClock.
    runner->PostTask(std::make_unique<vodka::JSTask>([] {}));
  }

 private:
  static std::atomic<double> time_;
};

std::atomic<double> FakeClock::time_{0.0};

/// 按顺序添加任务 -> 按顺序执行
TEST(JSTaskRunnerTest, PostTaskOrder) {
  vodka::JSTaskRunner runner{};
  std::vector<int> order;
  vodka::Semaphore semaphore(0);

  std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] { order.push_back(1); });
  std::shared_ptr<vodka::JSTask> task2 = std::make_shared<vodka::JSTask>([&] { order.push_back(2); });
  std::shared_ptr<vodka::JSTask> task3 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(3);
    semaphore.Signal();
  });

  runner.PostTask(std::move(task1));
  runner.PostTask(std::move(task2));
  runner.PostTask(std::move(task3));

  semaphore.Wait();

  runner.Quit();
  ASSERT_EQ(3UL, order.size());
  ASSERT_EQ(1, order[0]);
  ASSERT_EQ(2, order[1]);
  ASSERT_EQ(3, order[2]);
}

/// 按顺序添加延时任务 -> 按顺序执行

TEST(JSTaskRunnerTest, PostDelayedTaskOrder) {
  FakeClock::set_time(0.0);
  vodka::JSTaskRunner runner{};

  std::vector<int> order;
  vodka::Semaphore task1_semaphore(0);
  vodka::Semaphore task3_semaphore(0);

  std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(1);
    task1_semaphore.Signal();
  });
  std::shared_ptr<vodka::JSTask> task2 = std::make_shared<vodka::JSTask>([&] { order.push_back(2); });
  std::shared_ptr<vodka::JSTask> task3 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(3);
    task3_semaphore.Signal();
  });

  runner.PostDelayedTask(std::move(task1), 100);
  runner.PostTask(std::move(task2));
  runner.PostTask(std::move(task3));

  FakeClock::set_time_and_wake_up_runner(99, &runner);

  task3_semaphore.Wait();
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(2, order[0]);
  ASSERT_EQ(3, order[1]);

  FakeClock::set_time_and_wake_up_runner(101, &runner);
  task1_semaphore.Wait();

  runner.Quit();
  ASSERT_EQ(3UL, order.size());
  ASSERT_EQ(2, order[0]);
  ASSERT_EQ(3, order[1]);
  ASSERT_EQ(1, order[2]);
}


TEST(JSTaskRunnerTest, PostDelayedTaskOrder2) {
  FakeClock::set_time(0.0);
  vodka::JSTaskRunner runner{};

  std::vector<int> order;
  vodka::Semaphore task1_semaphore(0);
  vodka::Semaphore task2_semaphore(0);
  vodka::Semaphore task3_semaphore(0);

  std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(1);
    task1_semaphore.Signal();
  });
  std::shared_ptr<vodka::JSTask> task2 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(2);
    task2_semaphore.Signal();
  });
  std::shared_ptr<vodka::JSTask> task3 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(3);
    task3_semaphore.Signal();
  });

  runner.PostDelayedTask(std::move(task1), 500);
  runner.PostDelayedTask(std::move(task2), 100);
  runner.PostDelayedTask(std::move(task3), 200);

  FakeClock::set_time_and_wake_up_runner(101, &runner);

  task2_semaphore.Wait();
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(2, order[0]);

  FakeClock::set_time_and_wake_up_runner(201, &runner);

  task3_semaphore.Wait();
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(2, order[0]);
  ASSERT_EQ(3, order[1]);

  FakeClock::set_time_and_wake_up_runner(501, &runner);

  task1_semaphore.Wait();
  runner.Quit();
  ASSERT_EQ(3UL, order.size());
  ASSERT_EQ(2, order[0]);
  ASSERT_EQ(3, order[1]);
  ASSERT_EQ(1, order[2]);
}

//########## 退出测试
/**
 * 测试 case
 * 1. 退出之后的添加的任务不会被执行。
 * 2. 退出后执行所有能够执行的任务。（到时间的timer，未 cancel 的任务）
 * */
TEST(JSTaskRunnerTest, QuitRunnningRunner) {
  FakeClock::set_time(0.0);
  vodka::JSTaskRunner runner{};

  std::vector<int> order;
  vodka::Semaphore task1_semaphore(0);
  vodka::Semaphore task2_semaphore(0);
  vodka::Semaphore task3_semaphore(0);

  std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(1);
    task1_semaphore.Signal();
  });
  //未到时间的任务，不会被执行
  std::shared_ptr<vodka::JSTask> delay100Task = std::make_shared<vodka::JSTask>([&] {
    order.push_back(2);
    task2_semaphore.Signal();
  });

  //退出之后添加的任务，不会被执行
  std::shared_ptr<vodka::JSTask> task3 = std::make_shared<vodka::JSTask>([&] {
    order.push_back(3);
    task3_semaphore.Signal();
  });

  runner.PostTask(std::move(task1));
  runner.PostDelayedTask(std::move(delay100Task), 100);

  task1_semaphore.Wait();
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(1, order[0]);

  runner.Quit();
  FakeClock::set_time_and_wake_up_runner(201, &runner);
  // OK, we can't actually prove that this never executes. But wait a bit at
  // least.
  bool signalled =
      task2_semaphore.WaitFor(100);
  ASSERT_FALSE(signalled);
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(1, order[0]);
  //quit后加入的任务，不会被执行
  runner.PostTask(std::move(task3));
  signalled = task3_semaphore.WaitFor(100);
  ASSERT_FALSE(signalled);
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(1, order[0]);
}

/**
 * 测试 case: 退出正在 waiting 的 runner
 * */
TEST(JSTaskRunnerTest, QuitWaittingRunner) {

  std::vector<int> order;

  std::shared_ptr<vodka::JSTaskRunner> runner = std::make_shared<vodka::JSTaskRunner>();
  sleep(1);// check runner run

  runner->Quit();

  std::shared_ptr<vodka::JSTask> task = std::make_shared<vodka::JSTask>([&] {
    order.push_back(1);
  });
  runner->PostTask(std::move(task));
  ASSERT_EQ(0UL, order.size());
}

//########## 析构测试
/**
 * 测试 case：主线程析构(存在正在执行的任务)
 * */

TEST(JSTaskRunnerTest, DestoryOnMainThread) {
  FakeClock::set_time(0.0);
  std::vector<int> order;
  std::weak_ptr<vodka::JSTaskRunner> w_runner;
  {
    std::shared_ptr<vodka::JSTaskRunner> runner = std::make_shared<vodka::JSTaskRunner>();
    w_runner = runner;
    std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] {
      order.push_back(1);
      sleep(2);
    });

    std::shared_ptr<vodka::JSTask> task2 = std::make_shared<vodka::JSTask>([&] {
      order.push_back(2);
    });
    runner->PostTask(std::move(task1));
    runner->PostTask(std::move(task2));

    runner->Quit();
  }
// will be blocked
  ASSERT_TRUE(w_runner.expired());
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(1, order[0]);
  ASSERT_EQ(2, order[1]);

}
/**
 * 测试 case：JS线程析构(存在正在执行的任务)
 * */

TEST(JSTaskRunnerTest, DestoryOnJSThread) {
  FakeClock::set_time(0.0);
  std::vector<int> order;
  vodka::Semaphore task2_semaphore(0);
  std::weak_ptr<vodka::JSTaskRunner> w_runner;

  {
    std::shared_ptr<vodka::JSTaskRunner> runner = std::make_shared<vodka::JSTaskRunner>();
    w_runner = runner;
    std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] {
      order.push_back(1);
      sleep(2);
    });

    std::shared_ptr<vodka::JSTask> task2 = std::make_shared<vodka::JSTask>([runner, &order, &task2_semaphore] {
      order.push_back(2);
      task2_semaphore.Signal();
    });
    runner->PostTask(std::move(task1));
    runner->PostTask(std::move(task2));

    runner->Quit();
  }
  task2_semaphore.Wait();
  ASSERT_TRUE(w_runner.expired());
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(1, order[0]);
  ASSERT_EQ(2, order[1]);
}
/**
 * 测试 case：JS线程析构(存在正在执行的任务， 非阻塞)
 * */

TEST(JSTaskRunnerTest, DestoryOnJSThreadNoBlock) {
  FakeClock::set_time(0.0);
  std::vector<int> order;
  vodka::Semaphore task_semaphore(0);
  std::weak_ptr<vodka::JSTaskRunner> w_runner;

  {
    std::shared_ptr<vodka::JSTaskRunner> runner = std::make_shared<vodka::JSTaskRunner>();
    w_runner = runner;
    std::shared_ptr<vodka::JSTask> task1 = std::make_shared<vodka::JSTask>([&] {
      order.push_back(1);
      sleep(2);
    });

    std::shared_ptr<vodka::JSTask> task2 = std::make_shared<vodka::JSTask>([runner, &order, &task_semaphore] {
      order.push_back(2);
      task_semaphore.Signal();
    });
    runner->PostTask(std::move(task1));
    runner->PostTask(std::move(task2));

    runner->Quit();
  }
  // not block
  ASSERT_EQ(0UL, order.size());
  ASSERT_FALSE(w_runner.expired());
  task_semaphore.Wait();
  ASSERT_TRUE(w_runner.expired());
  ASSERT_EQ(2UL, order.size());
  ASSERT_EQ(1, order[0]);
  ASSERT_EQ(2, order[1]);
}

//########## 析构&退出 联合测试
/**
 * 测试 case：主线程退出，子线程析构
 * */
TEST(JSTaskRunnerTest, DestoryAndQuit) {

  std::vector<int> order;
  vodka::Semaphore task_semaphore(0);
  std::shared_ptr<vodka::JSTaskRunner> runner =
      std::make_shared<vodka::JSTaskRunner>();
  sleep(2);  // check runner sleeping
  std::shared_ptr<vodka::JSTask> task = std::make_shared<vodka::JSTask>([runner, &order, &task_semaphore] {
    order.push_back(1);
    task_semaphore.Signal();
  });
  runner->PostTask(std::move(task));
  runner->Quit();

  task_semaphore.Wait();
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(1, order[0]);
}

/**
 * 测试 case：主线程析，子线程退出
 * */

TEST(JSTaskRunnerTest, DestoryAndQuit2) {

  std::vector<int> order;
  {
    std::shared_ptr<vodka::JSTaskRunner> runner =
        std::make_shared<vodka::JSTaskRunner>();
    sleep(2);  // check runner sleeping
    std::shared_ptr<vodka::JSTask> task = std::make_shared<vodka::JSTask>([&order] {
      order.push_back(1);
    });
    runner->PostTask(std::move(task));
    //will be blocked
  }
  ASSERT_EQ(1UL, order.size());
  ASSERT_EQ(1, order[0]);
}

//########## 并发测试

TEST(JSTaskRunnerTest, ConcurrentOrder) {
  std::vector<int> order;
  int val = 0;
  {
    std::shared_ptr<vodka::JSTaskRunner> runner =
        std::make_shared<vodka::JSTaskRunner>();

    std::thread t1([&] {
      for (int i = 0; i < 20; ++i) {
        std::shared_ptr<vodka::JSTask> task =
            std::make_shared<vodka::JSTask>([&, i] {
              order.push_back(i);
              val++;
            });
        runner->PostTask(std::move(task));
      }
    });

    std::thread t2([&] {
      for (int i = 0; i < 20; ++i) {
        std::shared_ptr<vodka::JSTask> task =
            std::make_shared<vodka::JSTask>([&, i] {
              order.push_back(i);
              val++;
            });
        runner->PostTask(std::move(task));
      }
    });

    std::thread t3([&] {
      for (int i = 0; i < 20; ++i) {
        std::shared_ptr<vodka::JSTask> task =
            std::make_shared<vodka::JSTask>([&, i] {
              order.push_back(i);
              val++;
            });
        runner->PostTask(std::move(task));
      }
    });

    std::thread t4([&] {
      for (int i = 0; i < 20; ++i) {
        std::shared_ptr<vodka::JSTask> task =
            std::make_shared<vodka::JSTask>([&, i] {
              order.push_back(i);
              val++;
            });
        runner->PostTask(std::move(task));
      }
    });

    std::thread t5([&] {
      for (int i = 0; i < 20; ++i) {
        std::shared_ptr<vodka::JSTask> task =
            std::make_shared<vodka::JSTask>([&, i] {
              order.push_back(i);
              val++;
            });
        runner->PostTask(std::move(task));
      }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
  }

  ASSERT_EQ(100UL, order.size());
  ASSERT_EQ(100, val);
}