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
}
#endif  // VODKA_TIME_H
