//
// Created by didi on 2021/12/28.
//

#include "vodka/thread.h"
#include <string.h>

namespace vodka {

static void* ThreadEntry(void *arg){

  Thread *thread = static_cast<Thread *>(arg);
#ifdef OS_ANDROID
  pthread_setname_np(pthread_self(), thread->name());
#else
  pthread_setname_np(thread->name());
#endif
  thread->Run();
  return nullptr;
}
Thread::Thread(const Options &options){
  set_name(options.name());
}

void Thread::set_name(const char *name){
  strncpy(name_, name, sizeof(name_) - 1);
  name_[sizeof(name_) - 1] = '\0';
}

int Thread::Start(){

  pthread_attr_t attr;
  memset(&attr, 0, sizeof(attr));
  //On success, these functions return 0; on error, they return a nonzero error number.
  int result = pthread_attr_init(&attr);
  if (result != 0){
    return result;
  }
  result = pthread_create(&thread_, &attr, ThreadEntry, this);
  return result;
}

}