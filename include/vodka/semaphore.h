//
// Created by didi on 2022/1/19.
//

#ifndef VODKA_SEMAPHORE_H
#define VODKA_SEMAPHORE_H

#if VODKA_OS_MACOSX
#include <dispatch/dispatch.h>  // NOLINT
#else
#include <semaphore.h>  // NOLINT
#endif

namespace vodka {
/**
 * 仅用于单元测试
 * 目前 vodka 内部无同步操作要求。因此，同步操作由上层（平台）实现。
 * */
class Semaphore final {

 public:
  explicit Semaphore(int count);
  Semaphore(const Semaphore&) = delete;
  Semaphore& operator=(const Semaphore&) = delete;
  ~Semaphore();

  // Increments the semaphore counter.
  void Signal();

  // Decrements the semaphore counter if it is positive, or blocks until it
  // becomes positive and then decrements the counter.
  void Wait();

  bool WaitFor(uint64_t delay_in_milliseconds);

/// https://stackoverflow.com/questions/27736618/why-are-sem-init-sem-getvalue-sem-destroy-deprecated-on-mac-os-x-and-w
#if VODKA_OS_MACOSX
  using NativeHandle = dispatch_semaphore_t;
#else
  using NativeHandle = sem_t;
#endif

 private:
  NativeHandle native_handle_;
};
}

#endif  // VODKA_SEMAPHORE_H
