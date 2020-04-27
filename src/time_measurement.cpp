#include "time_measure/time_measurement.h"

#include <cassert>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

namespace TM {

TMString CtimeToString(const time_t t) {
  std::tm time_info;
  GetLocalTime(t, time_info);

  // %Y-%m-%d %H:%M:%S
  std::put_time(&time_info, "F% %T");

  std::ostringstream os;
  os << std::put_time(&time_info, "F% %T");

  return os.str();
}

void GetLocalTime(const time_t t, TMTimeStruct &tm) {
#ifdef _WIN32
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif
}

TMTimePoint GetCurrentTime() { return TMClock::now(); }

void PrintExecutionResult(const TMString &name, const TMTimePoint start_time) {
  auto stop_time_point = GetCurrentTime();

  std::tm time_info;
  GetLocalTime(std::chrono::system_clock::to_time_t(stop_time_point),
               time_info);

  static std::ofstream tm_log("time_measurement.log", std::ofstream::trunc);
  static std::mutex log_mutex;
  assert(tm_log.is_open());

  {
    std::lock_guard<std::mutex> guard(log_mutex);
    tm_log << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S") // date time
           << " [\"" << name << "\"] "
           << "execution time: "
           << std::chrono::duration_cast<std::chrono::milliseconds>(
                  stop_time_point - start_time)
                  .count()
           << "ms\n";
    tm_log.flush();
  }
}

TMString GetComplexName(TMString &&original_name) {
  std::ostringstream os;
  os << std::this_thread::get_id();
  return original_name + "@" + os.str();
}

BlockMeasurement::BlockMeasurement(TMString &&name) {
  name_ = GetComplexName(std::move(name));
  start_ = GetCurrentTime();
}

BlockMeasurement::~BlockMeasurement() { PrintExecutionResult(name_, start_); }

GeneralTimeMeasurement &GeneralTimeMeasurement::Instance() {
  static GeneralTimeMeasurement instance;
  return instance;
}

void GeneralTimeMeasurement::Start(std::string &&name) {
  // insert start time
  const auto it = measurement_map_.insert(
      std::make_pair(GetComplexName(std::move(name)), GetCurrentTime()));
  assert(it.second && "name should be unique");
}

void GeneralTimeMeasurement::Stop(std::string &&name) {
  auto it = measurement_map_.find(GetComplexName(std::move(name)));
  assert(it != measurement_map_.end() &&
         "you should call Start with the same name before call Stop");
  PrintExecutionResult(it->first, it->second);

  measurement_map_.erase(it);
}

} // namespace TM
