/**
 * @file time_measurement.h
 * @author tonghao.yuan
 * @brief functions for measuring execute time of a snippet of code
 * @version 0.1
 * @date 2019-09-19
 *
 */

#pragma once

#include <chrono>  // for precise timer
#include <string>
#include <unordered_map>

#ifdef NDEBUG
#  define EXPR_MEASURE(expression)
#  define BLOCK_MEASURE
#  define MEASURE_START(name)
#  define MEASURE_STOP(name)
#else
#  include <sstream>  // EXPR_MEASURE
#  define BLOCK_MEASURE TM::BlockMeasurement measure(__FUNCTION__)
#  define MEASURE_START(name) TM::GeneralTimeMeasurement::Instance().Start(name)
#  define MEASURE_STOP(name) TM::GeneralTimeMeasurement::Instance().Stop(name)

/// measure execution time of one exprssion, like a function call
/// @code{.cpp}
/// EXPR_MEASURE(func()) // measure the execution time of func
/// @endcode
#  define EXPR_MEASURE(expression)                             \
    {                                                          \
      std::stringstream ss;                                    \
      ss << __FILE__ << ":" << __LINE__ << " " << #expression; \
      MEASURE_START(ss.str());                                 \
      expression;                                              \
      MEASURE_STOP(ss.str());                                  \
    }

#endif  // NDEBUG

/**
 * Time measurement namespace
 */
namespace TM {

using TMClock      = std::chrono::system_clock;
using TMTimePoint  = std::chrono::time_point<std::chrono::system_clock>;
using TMString     = std::string;
using TMTimeStruct = std::tm;

/**
 * @brief helper functions
 * @{
 */
TMTimePoint GetCurrentTime();
void        GetLocalTime(const time_t t, TMTimeStruct &tm);
TMString    CtimeToString(const time_t t);
void        PrintExecutionResult(const TMString &name, const TMTimePoint start_time);

/**
 * @brief add a thread id to support multi-thread programs
 * @param original_name
 */
TMString GetComplexName(TMString &&original_name);
/**
 *@}
 */

/**
 * @brief used to measure block execution time
 *
 * put an object of this class in the block, it will measure the execution time from
 * that point to the block ends
 *
 * This is a convinent class used in block without call to Start/Stop in GeneralTimeMeasurement
 *
 * @code{.cpp}
 *
 * void func() {
 *    BlockMeasurement measure(__FUNCTION__); // start
 *    // code been measured
 *    // measurement end, measurement result will be printed to std::cout
 * }
 *
 * @endcode
 *
 */
class BlockMeasurement {
 public:
  BlockMeasurement(std::string &&name);
  ~BlockMeasurement();

 private:
  TMString       name_;  // name, used in output
  TMTimePoint    start_;
};

/**
 * @brief general purpose time measurement
 *
 * This is a singleton class, you can start from anywhere with call
 * to Start() and stop anywhere with call to Stop().
 *
 * @note name parameter in Start and Stop must be same to complete one measurement
 *
 * @code{.cpp}
 *
 * void func1() {
 *    GeneralTimeMeasurement::Instance().Start("test_measurement");
  *   // code been measured
 * }
 *
 * void func2() {
 *    GeneralTimeMeasurement::Instance().Stop("test_measurement");
  *  // code been measured
 * }
 *

 *
 * int main() {
 *    // measuremtn execution time from fun1 to fun2
 *    fun1();
 *    fun2();
 * }
 *
 * @endcode
 */
class GeneralTimeMeasurement {
 public:
  static GeneralTimeMeasurement &Instance();

  /**
   * @brief Call start to start measure, call stop to stop measure.
   * \p name in Start and Stop should be exactly the same, it is the identifier.
   * @{
   */
  void Start(std::string &&name);
  void Stop(std::string &&name);
  /**
   *@}
   */

 private:
  GeneralTimeMeasurement() = default;

 private:
  // name <-> start time
  std::unordered_map<TMString, TMTimePoint> measurement_map_;
};
}  // namespace TM
