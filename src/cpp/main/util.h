/**
 * @file   util.h
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
 * @brief   util.h includes some useful utilities
 */

#ifndef UTIL_H
#define UTIL_H

#include "logging.h"

#include <chrono>
#include <iomanip>
#include <iterator>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <fstream>


inline std::string getTimestamp() {
  // get a precise timestamp as a string
  const auto now = std::chrono::system_clock::now();
  const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
  const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()) % 1000;
  std::stringstream nowSs;
//  nowSs << std::put_time(std::localtime(&nowAsTimeT), "%c");
  nowSs
      << std::put_time(std::localtime(&nowAsTimeT), "%c")
      << '.' << std::setfill('0') << std::setw(3) << nowMs.count();

  return nowSs.str();
}

inline std::string get_date_string() {
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm tm;
  char buf[100] = {0};
  std::strftime(buf, sizeof(buf), "%Y-%m-%d  %H:%M:%S", std::localtime(&now));
  return std::string{buf};
}

/// Return the number of milliseconds since the steady clock epoch. NOTE: The
/// returned timestamp may be used for accurately measuring intervals but has
/// no relation to wall clock time. It must not be used for synchronization
/// across multiple nodes.
///
/// TODO(rkn): This function appears in multiple places. It should be
/// deduplicated.
///
/// \return The number of milliseconds since the steady clock epoch.
inline int64_t current_time_ms() {
  std::chrono::milliseconds ms_since_epoch =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::steady_clock::now().time_since_epoch());
  return ms_since_epoch.count();
}


/// A helper function to split a string by whitespaces.
///
/// \param str The string with whitespaces.
///
/// \return A vector that contains strings split by whitespaces.
inline std::vector<std::string> SplitStrByWhitespaces(const std::string &str) {
  std::istringstream iss(str);
  std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                  std::istream_iterator<std::string>());
  return result;
}

class InitShutdownRAII {
 public:
  /// Type of the Shutdown function.
  using ShutdownFunc = void (*)();

  /// Create an instance of InitShutdownRAII which will call shutdown
  /// function when it is out of scope.
  ///
  /// \param init_func The init function.
  /// \param shutdown_func The shutdown function.
  /// \param args The arguments for the init function.
  template <class InitFunc, class... Args>
  InitShutdownRAII(InitFunc init_func, ShutdownFunc shutdown_func, Args &&... args)
      : shutdown_(shutdown_func) {
    init_func(args...);
  }

  /// Destructor of InitShutdownRAII which will call the shutdown function.
  ~InitShutdownRAII() {
    if (shutdown_ != nullptr) {
      shutdown_();
    }
  }

 private:
  ShutdownFunc shutdown_;
};

struct EnumClassHash {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};

/// unordered_map for enum class type.
template <typename Key, typename T>
using EnumUnorderedMap = std::unordered_map<Key, T, EnumClassHash>;

/// A helper function to fill random bytes into the `data`.
/// Warning: this is not fork-safe, we need to re-seed after that.
template <typename T>
void FillRandom(T *data) {
  CHECK(data != nullptr);
  auto randomly_seeded_mersenne_twister = []() {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    // To increase the entropy, mix in a number of time samples instead of a single one.
    // This avoids the possibility of duplicate seeds for many workers that start in
    // close succession.
    for (int i = 0; i < 128; i++) {
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      seed += std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }
    std::mt19937 seeded_engine(seed);
    return seeded_engine;
  };

  // NOTE(pcm): The right way to do this is to have one std::mt19937 per
  // thread (using the thread_local keyword), but that's not supported on
  // older versions of macOS (see https://stackoverflow.com/a/29929949)
  static std::mutex random_engine_mutex;
  std::lock_guard<std::mutex> lock(random_engine_mutex);
  static std::mt19937 generator = randomly_seeded_mersenne_twister();
  std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint8_t>::max());
  for (int i = 0; i < data->size(); i++) {
    (*data)[i] = static_cast<uint8_t>(dist(generator));
  }
}

inline bool is_file_exist(const char *fileName)
{
  std::ifstream infile(fileName);
  return infile.good();
}

template <typename IN_T, typename OUT_T>
std::vector<OUT_T> linspace(IN_T start_in, IN_T end_in, int num_in)
{
  std::vector<OUT_T> linspaced;

  OUT_T start = static_cast<OUT_T>(start_in);
  OUT_T end = static_cast<OUT_T>(end_in);
  OUT_T num = static_cast<OUT_T>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1) {
    linspaced.push_back(start);
    return linspaced;
  }

  OUT_T delta = (end - start) / (num - 1);

  for(int i=0; i < num-1; ++i) {
    linspaced.push_back(start + delta * i);
  }
  linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
  return linspaced;
}

template <typename T>
std::vector<T> genrandvec(T start_in, T end_in, int num_in)
{
  std::vector<T> randvec;
  // engine only provides a source of randomness
//  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//  std::default_random_engine e(seed);


  std::random_device rd;
  std::mt19937 mt(rd());
  if (std::is_floating_point<T>::value) {
//    std::uniform_real_distribution<T> dist(start_in, end_in);
    std::uniform_real_distribution<double> dist(start_in, end_in);
    for(int i=0; i < num_in; ++i) {
      randvec.push_back(static_cast<T>(dist(mt)));
    }
  }
  else {
    std::uniform_int_distribution<T> dist(start_in, end_in);
    for(int i=0; i < num_in; ++i) {
      randvec.push_back(dist(mt));
    }
  }

  return randvec;
}

#endif  // UTIL_H
