//
// Created by didi on 2022/1/7.
//

#ifndef VODKA_TIME_H
#define VODKA_TIME_H

#pragma once
#include <chrono>
namespace vodka {

inline uint64_t SteadyClockNow(){
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch();
  auto result = std::chrono::duration_cast<std::chrono::milliseconds>(now_ms).count();
  return result;
}
/// 毫秒表示
struct TimeDelta {
 public:
  explicit TimeDelta(int64_t delta_ms) : delta_(delta_ms) ,stamp_(SteadyClockNow() + delta_ms){}
  static TimeDelta FromSeconds(int64_t delta) {
    return TimeDelta(delta * 1000);
  }
  static TimeDelta FromMilliseconds(int64_t delta) {
    return TimeDelta(delta);
  }

  inline int64_t delta() const {
    return delta_;
  }

  inline int64_t stamp() const {
    return stamp_;
  }
 private:
  int64_t delta_ = 0;
  uint64_t stamp_ = 0;
};

}
#endif  // VODKA_TIME_H
