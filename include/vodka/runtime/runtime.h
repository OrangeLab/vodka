//
// Created by didi on 2022/1/10.
//

#ifndef VODKA_RUNTIME_H
#define VODKA_RUNTIME_H

#include <functional>
#include <memory>
#include <atomic>
#include "context.h"
namespace vodka {

/**
 * Runtime 负责保证 context(及 context层级一下组件) 的线程安全。
 * 保证 context及(架构)以下：
 * 1. 只在 JS 线程访问(包括构造与析构)。
 * 2. 保证 context 指针正确性(访问到即为context存活)
 * -------------------------
 *        Runtime           | ----------
 * -------------------------|          ↓
 *        Context           | <---  JS Thread
 * -------------------------|
 * timers                   |
 * -------------------------|
 *
 * 引用关系：
 * 强引用 : ->
 * 弱引用 : ~>
 * runtime -> context
 * runtime -> taskRunner
 *
 * context -> timer
 * context -> taskRunner
 * context -> microTask

 * timer ~> context
 * timer -> delayTask(Task)
 *
 * taskRunner -> task
 *
 * task -> callback
 * task ~> context
 *
 * runtime 在析构之后，在重用 taskRunner 情况下，task 并不会被立即释放，这样设计(runtime析构，task仍然会执行)的原因如下：
 * 1. vodka 上层可以能会通过一些同步原语(semaphore等)，实现一些异步转同步的行为，如果此时不执行 task->callback，可能会导致调用线程一直sleep。
 * 2. context或runtime需要记录所有task。而对 task 的 cancel 的操作需要进行 加锁，可能会影响性能。
 * 因此会存在 callback 仍在被触发从而导致野指针(如 timer callback 捕获 this)。
 * 1. timer 通过 主动 cancel 来规避，具体见 timer.h
 * 2. vodka 上层业务，应在 RunOnJSThread callback 中，利用平台若引用特性，屏蔽该问题。
 *
 * 析构：
 * 1. runtime destory -> main thread
 *
 * 2. runtime free    -> js thread
 *    1. context free
 *    2. deref taskRunner
 * 3. context free
 *    1. free timers
 *    2. deref taskRunner
 * 4.timer free
 *    1. cancel dealyTask
 *
 * 5. taskRunner free
 *    1. task runner quit
 *    2. thread quit
 *
 * 线程安全：
 * 1. 保证类本身是线程安全的：方法，属性等访问不用出现 data race。-> taskRunner mutex
 * 2. 保证类在执行函数期间，对象不会在另一个线程被析构。          -> shared_ptr
 * 3. 调用某个对象的成员函数之前，要确保对象一定有效。           -> shared_ptr
 *
 * 扩展：
 * 目前 runtime 支持 RunOnJSThread 异步接口，如果要实现同步调用，平台扩展如下：
 * semaphore(0)
 * id result = nullptr;
 * runtime->RunOnJSThread([&semaphore]{
 *      // get reture value
 *      // result = call(xxx)
 *      // semaphore.signal()
 * })
 * semaphore.wait()
 * result is valid now
 * */

struct RuntimeConfig {
  bool micro_task_scope_ = false;
  bool thread_resuse_ = false;
};

class Runtime {
 public:
  explicit Runtime(RuntimeConfig config = RuntimeConfig());
  void Initialize();
  void Destroy();
  inline JSContext* context(){ return context_.get();}
  Runtime(const Runtime &) = delete;
  Runtime& operator=(const Runtime &) = delete;

  void RunOnJSThread(std::function<void()> &&callback);

 protected:
  std::shared_ptr<JSContext> context_;
  std::shared_ptr<JSTaskRunner> task_runner_;
  RuntimeConfig config_;
  std::atomic_bool is_destory_{false};
};
}

#endif  // VODKA_RUNTIME_H
