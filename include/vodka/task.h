//
// Created by didi on 2021/12/23.
//

#ifndef VODKA_TASK_H
#define VODKA_TASK_H

namespace vodka {
class Task {
 public:

  virtual ~Task(){};
  virtual void Run() {};

  bool canceled_ = false;
};


}

#endif  // VODKA_TASK_H
