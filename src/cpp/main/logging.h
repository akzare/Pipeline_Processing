/**
 * @file   logging.h
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
 * @brief   logging.h includes the implementation for the logger class
 */

#ifndef UTIL_LOGGING_H
#define UTIL_LOGGING_H

#include <iostream>
#include <fstream>
#include <string>

#if defined(_WIN32)
#ifndef _WINDOWS_
#ifndef WIN32_LEAN_AND_MEAN  // Sorry for the inconvenience. Please include any related
                             // headers you need manually.
                             // (https://stackoverflow.com/a/8294669)
#define WIN32_LEAN_AND_MEAN  // Prevent inclusion of WinSock2.h
#endif
#include <Windows.h>  // Force inclusion of WinGDI here to resolve name conflict
#endif
#ifdef ERROR  // Should be true unless someone else undef'd it already
#undef ERROR  // Windows GDI defines this macro; make it a global enum so it doesn't
              // conflict with our code
enum { ERROR = 0 };
#endif
#endif
namespace pl_proc {

struct tag_t;

enum class LogLevel { DEBUG = -1, INFO = 0, WARNING = 1, ERROR = 2, FATAL = 3 };

#define LOG_INTERNAL(level, log2file) ::pl_proc::PL_Log(__FILE__, __LINE__, level, log2file)

#define LOG_ENABLED(level) pl_proc::PL_Log::IsLevelEnabled(pl_proc::LogLevel::level)

#define LOG(level, log2file)                                      \
  if (pl_proc::PL_Log::IsLevelEnabled(pl_proc::LogLevel::level)) \
  LOG_INTERNAL(pl_proc::LogLevel::level, log2file)

#define IGNORE_EXPR(expr) ((void)(expr))

#define CHECK(condition)                                                          \
  (condition)                                                                     \
      ? IGNORE_EXPR(0)                                                            \
      : ::pl_proc::Voidify() & ::pl_proc::PL_Log(__FILE__, __LINE__, pl_proc::LogLevel::FATAL, false) \
                               << " Check failed: " #condition " "

#ifdef NDEBUG

#define DCHECK(condition)                                                         \
  (condition)                                                                     \
      ? IGNORE_EXPR(0)                                                            \
      : ::pl_proc::Voidify() & ::pl_proc::PL_Log(__FILE__, __LINE__, pl_proc::LogLevel::ERROR, false) \
                               << " Debug check failed: " #condition " "
#else

#define DCHECK(condition) CHECK(condition)

#endif  // NDEBUG


// To make the logging lib plugable with other logging libs and make
// the implementation unawared by the user, PL_Log is only a declaration
// which hide the implementation into logging.cc file.
// In logging.cc, we can choose different log libs using different macros.

// This is also a null log which does not output anything.
class LogBase {
 public:
  virtual ~LogBase(){};

  // By default, this class is a null log because it return false here.
  virtual bool IsEnabled() const { return false; };
  virtual bool IsFEnabled() const { return false; };

  template <typename T>
  LogBase &operator<<(const T &t) {
    if (IsEnabled()) {
      Stream() << t;
    }
    if (IsFEnabled()) {
   	  FStream() << t;
    }
    return *this;
  }

 protected:
  virtual std::ostream &Stream() { return std::cerr; };
  virtual std::ostream &FStream() = 0;
};

class PL_Log : public LogBase {
 public:
  PL_Log(const char *file_name, int line_number, LogLevel severity, bool IsLogFile);
  static void LogTag(tag_t tag);

  virtual ~PL_Log();

  /// Return whether or not current logging instance is enabled.
  ///
  /// \return True if logging is enabled and false otherwise.
  virtual bool IsEnabled() const;
  virtual bool IsFEnabled() const;

  /// The init function of pl_proc log for a program which should be called only once.
  ///
  /// \parem appName The app name which starts the log.
  /// \param severity_threshold Logging threshold for the program.
  /// \param logDir Logging output file name. If empty, the log won't output to file.
  static void StartLog(const std::string &appName,
                       LogLevel severity_threshold = LogLevel::INFO,
                       const std::string &logDir = "");

  /// Return whether or not the log level is enabled in current setting.
  ///
  /// \param log_level The input log level to test.
  /// \return True if input log level is not lower than the threshold.
  static bool IsLevelEnabled(LogLevel log_level);

 private:
  // Hide the implementation of log provider by void *.
  // Otherwise, lib user may define the same macro to use the correct header file.
  void *logging_provider_;
  /// True if log messages should be logged and false if they should be ignored.
  bool is_enabled_;
  bool is_fenabled_;
  static LogLevel severity_threshold_;
  // In InitGoogleLogging, it simply keeps the pointer.
  // We need to make sure the app name passed to InitGoogleLogging exist.
  static std::string app_name_;
  /// The directory where the log files are stored.
  /// If this is empty, logs are printed to stdout.
  static std::string log_dir_;

  static std::ofstream logfile_;
  static std::ofstream eventLogfile_;

 protected:
  virtual std::ostream &Stream();
  virtual std::ostream &FStream();
};

// This class make check compilation pass to change the << operator to void.
// This class is copied from glog.
class Voidify {
 public:
  Voidify() {}
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(LogBase &) {}
};

}  // namespace pl_proc

#endif  // UTIL_LOGGING_H
