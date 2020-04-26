#include "time_measure/time_measurement.h"

#include <chrono>
#include <iostream>
#include <thread>

void hello() { EXPR_MEASURE(std::cout << "Hello World!" << std::endl;) }

void sleep_1() {
  BLOCK_MEASURE;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return;
}

void sleep_2() { std::this_thread::sleep_for(std::chrono::seconds(2)); }

int main() {
  hello();
  sleep_1();

  MEASURE_START("meas1");
  sleep_2();
  MEASURE_STOP("meas1");
}
// output:
// 2019-10-16 16:39:46 ["D:\open_source_project\time_measure\example.cpp:8
// std::cout << "Hello World!" << std::endl;@9904"] execution time: 0ms
// 2019-10-16 16:39:47 ["sleep_1@9904"] execution time: 1000ms
// 2019-10-16 16:39:49 ["meas1@9904"] execution time: 2000ms
