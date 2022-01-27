//
// Created by didi on 2022/1/19.
//
#include "vodka/semaphore.h"

#if VODKA_OS_MACOSX
#include <dispatch/dispatch.h>
#endif
#include <errno.h>

namespace vodka {

#if VODKA_OS_MACOSX

Semaphore::Semaphore(int count) {
  native_handle_ = dispatch_semaphore_create(count);
}

Semaphore::~Semaphore() { dispatch_release(native_handle_); }

void Semaphore::Signal() { dispatch_semaphore_signal(native_handle_); }

void Semaphore::Wait() {
  dispatch_semaphore_wait(native_handle_, DISPATCH_TIME_FOREVER);
}

bool Semaphore::WaitFor(uint64_t delay_in_milliseconds) {
  dispatch_time_t timeout =
      dispatch_time(DISPATCH_TIME_NOW, delay_in_milliseconds * NSEC_PER_MSEC);
  return dispatch_semaphore_wait(native_handle_, timeout) == 0;
}

#else

Semaphore::Semaphore(int count) {
  int result = sem_init(&native_handle_, 0, count);
}


Semaphore::~Semaphore() {
  int result = sem_destroy(&native_handle_);
}

void Semaphore::Signal() {
  int result = sem_post(&native_handle_);
  // This check may fail with <libc-2.21, which we use on the try bots, if the
  // semaphore is destroyed while sem_post is still executed. A work around is
  // to extend the lifetime of the semaphore.
  if (result != 0) {
  }
}


void Semaphore::Wait() {
  while (true) {
    int result = sem_wait(&native_handle_);
    if (result == 0) return;  // Semaphore was signalled.
    // Signal caused spurious wakeup.
  }
}
#endif
}