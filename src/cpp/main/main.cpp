/**
 * @file   main.cpp
 * @author Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 * @brief   main.cpp develops the main module for
 *          Pipeline Processing Framework.
 * 
 */

#include "util.h"
#include "logging.h"
#include "sys_builder.h"

#include <memory>
#include <iostream>
#include <limits>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <fstream>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


const char* cfg_file_name = "C:\\Users\\akzare\\github\\Pipeline_Processing\\config\\sample_pipeline.json";
const char* log_file_name = "C:\\Users\\akzare\\Pipeline_Processing\\log\\";


int main()
{
  pl_proc::PL_Log::StartLog("pl_proc", pl_proc::LogLevel::DEBUG, log_file_name);

  LOG(INFO, true) << ", main, Pipeline Processing Framework Starts..." << "\n";

  try {
    pl_proc::sys_builder sysBuilder(cfg_file_name);
    sysBuilder.print_pipeline();
    sysBuilder.connect_pipeline_proc();
    sysBuilder.connect_pipeline_2_logger();
    sysBuilder.run_sim();
  }
  catch (const std::invalid_argument& ia) {
    std::cerr << "Invalid argument: " << ia.what() << '\n';
  }
  catch (const std::runtime_error& ia) {
    std::cerr << "Invalid argument: " << ia.what() << '\n';
  }
  catch (...)  {
    std::cout << "Default Exception\n";
  }

  LOG(INFO, true) << ", main, Pipeline Processing Framework Ends." << "\n";

  return 0;
}
