//
// Created by didi on 2021/12/28.
//

#ifndef VODKA_THREAD_H
#define VODKA_THREAD_H

#pragma once
#include <pthread.h>

namespace vodka {

class Thread {
 public:
  class Options {
   public:
    Options() :name_("hummer:<unknown>") {}
    explicit Options(const char *name) : name_(name) {}

    const char *name() const {return name_;};
   private:
    const char *name_;
  };

  explicit Thread(const Options &options);
  virtual ~Thread() {};

  Thread(const Thread &) = delete;
  Thread& operator=(const Thread&) = delete;

  // Start new thread by calling the Run() method on the new thread.
  // On success, pthread_create() returns 0; on error, it returns an error number
  int Start();
  virtual void Run() = 0;

  inline const char* name() const { return name_; }

  // The thread name length is limited to 16 based on Linux's implementation of
  // https://man7.org/linux/man-pages/man3/pthread_setname_np.3.html
  static const int kMaxThreadNameLength = 16;
 private:
  void set_name(const char* name);
  char name_[kMaxThreadNameLength];

  pthread_t thread_;
};

}

#endif  // VODKA_THREAD_H
