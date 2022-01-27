//
// Created by didi on 2021/12/24.
//

#include "vodka/js_task_runner.h"
#include "vodka/time.h"
namespace vodka {

JSTask::JSTask(Function &&func, const std::shared_ptr<JSContext> &context) :callback_(std::move(func)), context_(context) { }


void JSTask::Run() {
  if (callback_){
    // todo：bad_function_call
    callback_();
  }
}
std::weak_ptr<JSTaskRunner> GlobalJSTaskRunner::reuse_instance_;
std::shared_ptr<JSTaskRunner> GlobalJSTaskRunner::GetInstance(){

  std::shared_ptr<JSTaskRunner> runner = reuse_instance_.lock();
  if (runner == nullptr || reuse_instance_.expired()){
    runner = std::make_shared<JSTaskRunner>();
    reuse_instance_ = runner;
  }
  return runner;
}

JSTaskRunner::JSThread::JSThread(JSTaskRunner *task_runner) : Thread(Thread::Options("hummer.vodka.js.taskRunner")),
                                                              taskRunner_(task_runner),
                                                              no_runner(task_runner == nullptr){
  Start();
}

void JSTaskRunner::JSThread::Run(){

  while (no_runner == false) {
    std::shared_ptr<Task> task = taskRunner_->GetNext();
    if (task == nullptr){
      return;
    }
    task->Run();
  }
}

JSTaskRunner::JSTaskRunner():thread_(std::make_unique<JSThread>(this)) { }

// 如果 JSTaskRunner 在非 JSThread 析构，则无法保证 task runner 的线程安全：函数执行过程中被析构。
// 因此 通过 Jion 阻止析构过程，直到退出事件循环
JSTaskRunner::~JSTaskRunner() {
  is_quit_ = true;
  pthread_t js_thread = thread_->thread();
  if (pthread_equal(pthread_self(), js_thread) == 0){
    //不在 js 线程。
    cv_.notify_one();
    thread_->Join();
  }
  thread_->no_runner = true;
}

void JSTaskRunner::PostTask(std::shared_ptr<Task> task) {

  if (is_quit_){ return; }
  std::lock_guard<std::mutex> lock(mutex_);
  task_queue_.push(std::move(task));
  cv_.notify_one();
}

void JSTaskRunner::PostDelayedTask(std::shared_ptr<Task> task, int64_t delay_in_milliseconds) {

  if (is_quit_){ return; }
  std::lock_guard<std::mutex> lock(mutex_);
  if (delay_in_milliseconds <= 0){
    task_queue_.push(std::move(task));
  }else{
    delayed_task_queue_.push(DelayedPair(TimeDelta::FromMilliseconds(delay_in_milliseconds), std::move(task)));
  }
  cv_.notify_one();
}

std::shared_ptr<Task> JSTaskRunner::GetNext() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (true) {
    // check timeout
    auto now = SteadyClockNow();
    while (true) {
      if (delayed_task_queue_.empty()) {break;}
      const DelayedPair &delayedTask = delayed_task_queue_.top();
      if (delayedTask.first.stamp() > now) {break;}
      if (!delayedTask.second->canceled_) {
        // remove cv
        DelayedPair &no_const_delayedTask = const_cast<DelayedPair &>(delayedTask);
        task_queue_.push(std::move(no_const_delayedTask.second));
      }
      delayed_task_queue_.pop();
    }

    {
      // get runnable task
      while (true) {
        if (task_queue_.empty()) {
          break;
        }
        std::shared_ptr<Task> &refTask = task_queue_.front();
        if (!refTask->canceled_) {
          std::shared_ptr<Task> task = std::move(refTask);
          task_queue_.pop();
          return task;
        }
        task_queue_.pop();
      }
    }
    if (is_quit_){
      return nullptr;
    }
    {
      // wait signal
      if (task_queue_.empty() && !delayed_task_queue_.empty()) {
        // wakeup for timeout
        const DelayedPair &delayed_task = delayed_task_queue_.top();
        uint64_t wakeTime = delayed_task.first.stamp() - now;
        next_wake_up_time_ = delayed_task.first;
        cv_.wait_for(lock, std::chrono::milliseconds(wakeTime));
      }else{
        next_wake_up_time_ = TimeDelta{0};
        cv_.wait(lock);
      }
    }
  }
}

void JSTaskRunner::Quit() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (is_quit_){ return; }
  is_quit_ = true;
  cv_.notify_one();
}


}