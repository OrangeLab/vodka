//
// Created by didi on 2022/1/21.
//
#include <gtest/gtest.h>
#include "vodka/runtime/context.h"
#include "vodka/runtime/timer.h"
#include "vodka/semaphore.h"

class TimerTaskRunner : public vodka::JSTaskRunner {
 public:
  void RunUntil(uint64_t ms) {
    std::shared_ptr<vodka::JSTask> task = std::make_shared<vodka::JSTask>([&] {
      semaphore.Signal();
    });
    PostDelayedTask(std::move(task), ms);
    semaphore.Wait();
  }

 private:
  vodka::Semaphore semaphore{0};
};

class TimerTest : public testing::Test {
 public:

  void SetUp() override {
    run_times_.clear();
    runner_ = std::make_shared<TimerTaskRunner>();
  }
  std::vector<uint64_t> run_times_;
 protected:
  uint64_t start_time_;
  std::vector<uint64_t> next_fire_times_;
  std::shared_ptr<TimerTaskRunner> runner_;

};

class TestTimer : public vodka::TimerBase{
 public:
  explicit TestTimer(TimerTaskRunner *runner, bool single_shot, TimerTest *tt) : TimerBase(runner), tt_(tt){ }
  TimerTest *tt_;
  void Fired() override {
    tt_->run_times_.push_back(this->delay_in_millisecond());
  }
};

//########## set timeout 0
// timeout 0
TEST_F(TimerTest, StartOneShot_Zero) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(0);
  vodka::TimeDelta run_time{0};
  runner_->RunUntil(0);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(0, run_times_[0]);
}

// timeout 0 - cancel
TEST_F(TimerTest, StartOneShot_ZeroAndCancel) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(0);
  vodka::TimeDelta run_time{0};
  timer.Stop();
  runner_->RunUntil(0);
  ASSERT_EQ(0UL, run_times_.size());
}

// timeout 0 - cancel & repost
TEST_F(TimerTest, StartOneShot_ZeroAndCancelThenRepost) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(0);
  vodka::TimeDelta run_time{0};
  timer.Stop();
  runner_->RunUntil(0);
  ASSERT_EQ(0UL, run_times_.size());

  timer.StartOneShot(0);

  runner_->RunUntil(0);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(0, run_times_[0]);
}

// timeout 0 - running & repost
TEST_F(TimerTest, StartOneShot_Zero_RepostingAfterRunning) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(0);
  vodka::TimeDelta run_time{0};
  runner_->RunUntil(0);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(0, run_times_[0]);

  // repost
  timer.StartOneShot(0);
  vodka::TimeDelta run_time1{0};
  runner_->RunUntil(0);
  ASSERT_EQ(2UL, run_times_.size());
  ASSERT_EQ(0, run_times_[0]);
  ASSERT_EQ(0, run_times_[1]);
}


//########## set timeout 非0
// timeout 10
TEST_F(TimerTest, StartOneShot_NonZero) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(10);
  vodka::TimeDelta run_time{0};
  runner_->RunUntil(10);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(10, run_times_[0]);
}

// timeout 10 - cancel

TEST_F(TimerTest, StartOneShot_NonZeroAndCancel) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(10);
  vodka::TimeDelta run_time{0};
  timer.Stop();
  runner_->RunUntil(10);
  ASSERT_EQ(0UL, run_times_.size());
}

// timeout 10 - cancel & repost
TEST_F(TimerTest, StartOneShot_NonZeroAndCancelThenRepost) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(10);
  vodka::TimeDelta run_time{0};
  timer.Stop();
  runner_->RunUntil(10);
  ASSERT_EQ(0UL, run_times_.size());

  timer.StartOneShot(10);

  runner_->RunUntil(10);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(10, run_times_[0]);
}

// timeout 0 - running & repost
TEST_F(TimerTest, StartOneShot_NonZero_RepostingAfterRunning) {


  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(10);
  vodka::TimeDelta run_time{0};
  runner_->RunUntil(10);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(10, run_times_[0]);

  // repost
  timer.StartOneShot(10);
  vodka::TimeDelta run_time1{0};
  runner_->RunUntil(10);
  ASSERT_EQ(2UL, run_times_.size());
  ASSERT_EQ(10, run_times_[0]);
  ASSERT_EQ(10, run_times_[1]);
}
//########## set timeout post twice
TEST_F(TimerTest, PostingTimerTwiceWithSameRunTimeDoesNothing) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(10);
  timer.StartOneShot(10);
  vodka::TimeDelta run_time{0};

  runner_->RunUntil(21);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(10, run_times_[0]);
}
TEST_F(TimerTest, PostingTimerTwiceWithLaterRunTimeCancelsOriginalTask) {

  TestTimer timer{runner_.get(), true, this};
  timer.StartOneShot(10);
  timer.StartOneShot(20);
  vodka::TimeDelta run_time{0};

  runner_->RunUntil(21);
  ASSERT_EQ(1UL, run_times_.size());
  ASSERT_EQ(20, run_times_[0]);
}
//########## set interval
TEST_F(TimerTest, StartRepeatingTask) {

  TestTimer timer{runner_.get(), false, this};
  timer.StartRepeating(1000);
  vodka::TimeDelta run_time{0};

  runner_->RunUntil(5500);
  ASSERT_EQ(5UL, run_times_.size());
  ASSERT_EQ(1000, run_times_[0]);
  ASSERT_EQ(1000, run_times_[1]);
  ASSERT_EQ(1000, run_times_[2]);
  ASSERT_EQ(1000, run_times_[3]);
  ASSERT_EQ(1000, run_times_[4]);
}

TEST_F(TimerTest, StartRepeatingTask_ThenCancel) {

  TestTimer timer{runner_.get(), false, this};
  timer.StartRepeating(1000);
  vodka::TimeDelta run_time{0};
  runner_->RunUntil(2500);
  ASSERT_EQ(2UL, run_times_.size());
  ASSERT_EQ(1000, run_times_[0]);
  ASSERT_EQ(1000, run_times_[1]);

  //stop
  timer.Stop();
  runner_->RunUntil(2500);
  ASSERT_EQ(2UL, run_times_.size());
  ASSERT_EQ(1000, run_times_[0]);
  ASSERT_EQ(1000, run_times_[1]);
}

TEST_F(TimerTest, StartRepeatingTask_ThenPostOneShot) {

  TestTimer timer{runner_.get(), false, this};
  timer.StartRepeating(1000);
  vodka::TimeDelta run_time{0};
  runner_->RunUntil(2500);
  ASSERT_EQ(2UL, run_times_.size());
  ASSERT_EQ(1000, run_times_[0]);
  ASSERT_EQ(1000, run_times_[1]);

  //stop
  timer.StartOneShot(0);
  runner_->RunUntil(0);
  ASSERT_EQ(3UL, run_times_.size());
  ASSERT_EQ(1000, run_times_[0]);
  ASSERT_EQ(1000, run_times_[1]);
  ASSERT_EQ(0, run_times_[2]);
}