/**
 * @file   logging.cpp
 *
 * Copyright 2017 The Ray Authors. https://github.com/ray-project/ray
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   logging.cpp includes the implementation for the logger class
 */

#include "logging.h"
#include "id.h"
#include "tags.h"
#include "util.h"

#ifndef _WIN32
#include <execinfo.h>
#endif

#include <signal.h>
#include <stdlib.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>


namespace pl_proc {

// This is the default implementation of pl_proc log,
// which is independent of any libs.
class CerrLog {
 public:
  CerrLog(LogLevel severity) : severity_(severity), has_logged_(false) {}

  virtual ~CerrLog() {
    if (has_logged_) {
      std::cerr << std::endl;
    }
    if (severity_ == LogLevel::FATAL) {
      PrintBackTrace();
      std::abort();
    }
  }

  std::ostream &Stream() {
    has_logged_ = true;
    return std::cerr;
  }

  template <class T>
  CerrLog &operator<<(const T &t) {
    if (severity_ != LogLevel::DEBUG) {
      has_logged_ = true;
      std::cerr << t;
    }
    return *this;
  }

 protected:
  const LogLevel severity_;
  bool has_logged_;

  void PrintBackTrace() {
#if defined(_EXECINFO_H) || !defined(_WIN32)
    void *buffer[255];
    const int calls = backtrace(buffer, sizeof(buffer) / sizeof(void *));
    backtrace_symbols_fd(buffer, calls, 1);
#endif
  }
};

typedef pl_proc::CerrLog LoggingProvider;

LogLevel PL_Log::severity_threshold_ = LogLevel::INFO;
std::string PL_Log::app_name_ = "";
std::string PL_Log::log_dir_ = "";
std::ofstream PL_Log::logfile_;
std::ofstream PL_Log::eventLogfile_;


void PL_Log::StartLog(const std::string &app_name, LogLevel severity_threshold,
                         const std::string &log_dir) {
  severity_threshold_ = severity_threshold;
  app_name_ = app_name;
  log_dir_ = log_dir;
}

bool PL_Log::IsLevelEnabled(LogLevel log_level) {
  return log_level >= severity_threshold_;
}

PL_Log::PL_Log(const char *file_name, int line_number, LogLevel severity, bool IsLogFile)
    // glog does not have DEBUG level, we can handle it using is_enabled_.
    : logging_provider_(nullptr), is_enabled_(severity >= severity_threshold_), is_fenabled_(IsLogFile) {
  auto logging_provider = new CerrLog(severity);
  if (is_enabled_){
    if (severity == LogLevel::WARNING || severity == LogLevel::ERROR){
      *logging_provider << get_date_string() << " " << file_name << ":" << line_number << ": ";
    } else {
      *logging_provider << getTimestamp() << " ";
    }
  }

  logging_provider_ = logging_provider;

  if (!logfile_.is_open())
  {
    JobRunID *jobRunId = jobRunId->getInstance();
    std::string logFilename = log_dir_+ "pl_" + static_cast<std::stringstream>(*jobRunId).str() + ".log";
    logfile_.open(logFilename.c_str(), std::ios::out | std::ios::app);
    if (logfile_.fail()) {
      LOG(FATAL, false) << "Open logfile failure: " << logFilename;
    } else {
      LOG(INFO, false) << "Logging into:" << logFilename;
    }
  }

  if (!eventLogfile_.is_open())
  {
    JobRunID *jobRunId = jobRunId->getInstance();
    std::string logFilename = log_dir_+ "pl_event_" + static_cast<std::stringstream>(*jobRunId).str() + ".log";
    eventLogfile_.open(logFilename.c_str(), std::ios::out | std::ios::app);
    if (eventLogfile_.fail()) {
      LOG(FATAL, false) << "Open event logfile failure: " << logFilename;
    } else {
      LOG(INFO, false) << "Logging events into:" << logFilename;
    }

    eventLogfile_ <<
  	        "TimeStamp" <<
  	        ", TimeTag" <<
  	        ", RunID"  <<
  	        ", ModuleTyp" <<
  	        ", ModuleIdx" <<
  	        ", PaketIdx" <<
  	        ", DataTyp" <<
  	        ", Data" <<
  	        "\n";
  }

  if (is_fenabled_) {
    if (severity == LogLevel::WARNING || severity == LogLevel::ERROR){
      logfile_ << get_date_string() << file_name << ":" << line_number << ": ";
    }else{
      logfile_ << getTimestamp() << " ";
    }
  }
}

std::string moduleType2String(ObjectIDModuleType moduleType)
{
  std::stringstream os;

  switch (moduleType)
  {
  case static_cast<ObjectIDModuleType>(ModuleType::ADDER_MODULE):
  {
    os << "ADDER";
    break;
  }
  case static_cast<ObjectIDModuleType>(ModuleType::SRC_NOISE_MODULE):
  {
    os << "SRC_NOISE";
    break;
  }
  case static_cast<ObjectIDModuleType>(ModuleType::SRC_VEC_MODULE):
  {
    os << "SRC_VEC";
    break;
  }
  case static_cast<ObjectIDModuleType>(ModuleType::SINK_VEC_MODULE):
  {
    os << "SINK_VEC";
    break;
  }
  default:
    LOG(ERROR, true) << "ModuleType is unknown";
    os << "UNKNOWN";
  }

 return os.str();
}

void PL_Log::LogTag(tag_t tag) {

  eventLogfile_ <<
		  getTimestamp() <<
		  ", " << tag.timetag_ <<
		  ", "  << tag.key_.GetRunID() <<
		  ", " << moduleType2String(tag.key_.GetModuleType()) <<
		  ", " << unsigned(tag.key_.GetModuleIndex()) <<
		  ", " << static_cast<uint32_t>(tag.key_.GetPaketIndex()) <<
		  ", ";

  std::stringstream os;

  switch (tag.valueDataType_)
  {
  case pmt::DataType::GVEC_UINT8:
  {
    os << "GVEC_UINT8, ";
    const std::vector<uint8_t> data = pmt::genVector_elements<uint8_t>(tag.value_);
    for (auto i: data) os << unsigned(i) << ", ";
    break;
  }
  case (pmt::DataType::GVEC_INT8):
  {
    os << "GVEC_INT8, ";
    const std::vector<int8_t> data = pmt::genVector_elements<int8_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_UINT16:
  {
    os << "GVEC_UINT16, ";
    const std::vector<uint16_t> data = pmt::genVector_elements<uint16_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_INT16:
  {
    os << "GVEC_INT16, ";
    const std::vector<int16_t> data = pmt::genVector_elements<int16_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_INT32:
  {
    os << "GVEC_INT32, ";
    const std::vector<int32_t> data = pmt::genVector_elements<int32_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_UINT32:
  {
    os << "GVEC_UINT32, ";
    const std::vector<uint32_t> data = pmt::genVector_elements<uint32_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_INT64:
  {
    os << "GVEC_INT64, ";
    const std::vector<int64_t> data = pmt::genVector_elements<int64_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_UINT64:
  {
    os << "GVEC_UINT64, ";
    const std::vector<uint64_t> data = pmt::genVector_elements<uint64_t>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_FLOAT:
  {
    os << "GVEC_FLOAT, ";
    const std::vector<float> data = pmt::genVector_elements<float>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_DOUBLE:
  {
    os << "GVEC_DOUBLE, ";
    const std::vector<float> data = pmt::genVector_elements<float>(tag.value_);
    for (auto i: data) os << i << ", ";
    break;
  }
  case pmt::DataType::GVEC_COMPLEX_FLOAT:
  {
    os << "GVEC_COMPLEX_FLOAT, ";
    const std::vector<std::complex<float>> data = pmt::genVector_elements<std::complex<float>>(tag.value_);
    for (auto i: data) os << "(" << i.real() << ":" << i.imag() << "), ";
    break;
  }
  case pmt::DataType::GVEC_COMPLEX_DOUBLE:
  {
    os << "GVEC_COMPLEX_DOUBLE, ";
    const std::vector<std::complex<double>> data = pmt::genVector_elements<std::complex<double>>(tag.value_);
    for (auto i: data) os << "(" << i.real() << ":" << i.imag() << "), ";
    break;
  }
  default:
    os << "UNKNOWN, ";
    LOG(ERROR, true) << "pmt::DataType is unknown";
  }

  eventLogfile_ << os.str();

  eventLogfile_ << "\n";
}

std::ostream &PL_Log::Stream() {
  auto logging_provider = reinterpret_cast<LoggingProvider *>(logging_provider_);
  return logging_provider->Stream();
}

std::ostream &PL_Log::FStream() {
  return logfile_;
}

bool PL_Log::IsEnabled() const { return is_enabled_; }
bool PL_Log::IsFEnabled() const { return is_fenabled_; }

PL_Log::~PL_Log() {
  if (logging_provider_ != nullptr) {
    delete reinterpret_cast<LoggingProvider *>(logging_provider_);
    logging_provider_ = nullptr;
  }
}

}  // namespace pl_proc
