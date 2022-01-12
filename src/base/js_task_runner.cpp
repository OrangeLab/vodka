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

JSTaskRunner::JSThread::JSThread(JSTaskRunner *task_runner) : Thread(Thread::Options("hummer.vodka.js.taskRunner")), taskRunner_(task_runner) {
  Start();
}

void JSTaskRunner::JSThread::Run(){

  taskRunner_->Run();
}

JSTaskRunner::JSTaskRunner() {

  thread_ = std::make_unique<JSThread>(this);
}

JSTaskRunner::~JSTaskRunner() {
  Quit();
}

void JSTaskRunner::PostTask(std::shared_ptr<Task> task) {

  if (is_quit_){ return; }
  std::lock_guard<std::mutex> lock(mutex_);
  task_queue_.push(std::move(task));
}

void JSTaskRunner::PostDelayedTask(std::shared_ptr<Task> task, int64_t delay_in_milliseconds) {

  if (is_quit_){ return; }
  if (delay_in_milliseconds <= 0){
    std::lock_guard<std::mutex> lock(mutex_);
    task_queue_.push(std::move(task));
  }else{
    auto fireTime = SteadyClockNow() + delay_in_milliseconds;
    delayed_task_queue_.push(DelayedPair(fireTime, std::move(task)));
  }
}

std::shared_ptr<Task> JSTaskRunner::GetNext() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (true) {
    // check timeout
    auto now = SteadyClockNow();
    while (true) {
      if (delayed_task_queue_.empty()) {break;}
      const DelayedPair &delayedTask = delayed_task_queue_.top();
      if (delayedTask.first > now) {break;}
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

    {
      // wait signal
      if (task_queue_.empty() && !delayed_task_queue_.empty()) {
        // wakeup for timeout
        const DelayedPair &delayed_task = delayed_task_queue_.top();
        uint64_t wakeTime = delayed_task.first - now;
        cv_.wait_for(lock, std::chrono::milliseconds(wakeTime));

      }else{
        cv_.wait(lock);
      }
    }
  }
}

void JSTaskRunner::Run() {
  while (true){
    if (is_quit_){return;}
    auto task = GetNext();
    task->Run();
  }
}
void JSTaskRunner::Quit() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (is_quit_){ return; }
  is_quit_ = true;

}

}