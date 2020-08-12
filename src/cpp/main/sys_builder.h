/**
 * @file   sys_builder.h
 *
 * @author Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *         Eric J. Mayo eric@pozicom.net
 *
 * @brief   sys_builder.h includes the system builder for the 
 *          Pipeline Processing Framework.
 */

#ifndef SYS_BUILDER_H
#define SYS_BUILDER_H

#include "noncopyable.h"
#include "logging.h"
#include "het_container.h"
#include "processor_factory.h"

#include <iosfwd>
#include <vector>
#include <iostream>

namespace pl_proc {

struct het_container_print_processor : het_container_visitor_base<processor::sptr>
{
  template<class T>
  void operator()(T& _in)
  {
    LOG(INFO, true) << ", het_container_print_processor, " << _in << "\n";
  }
};

struct het_container_connect_2_logger : het_container_visitor_base<processor::sptr>
{
  template<class T>
  void operator()(T& _in)
  {
    for (auto const& i : _in->getAdjacencyConnection()) {
      if(std::get<0>(i) == "logger"){
        _in->getOnNewTag()->connect(&PL_Log::LogTag);
        LOG(INFO, true) << ", het_container_connect_2_logger, Connect " << _in->getModuleName().c_str() << " to logger\n";
      }
    }
  }
};

struct het_container_connect_processors : het_container_visitor_base<processor::sptr>
{
  template<class T>
  void operator()(std::vector<T>& _in)
  {
    for (auto const& i : _in) {
      for (auto const& j : i->getAdjacencyConnection()) {
        std::string procName = std::get<0>(j);
        std::string sigName = std::get<1>(j);
        std::string funName = std::get<2>(j);
//        std::tie(procName, sigName, funName) = j;
        if(procName != "logger"){
          for (auto const& k : _in) {
            if(procName == k->getModuleName() && i->getModuleName() != k->getModuleName()) {
              if (sigName == "NewData" && funName == "Proc") {
                i->getOnNewDataGen()->template connect_member<processor>(k, &processor::process);
                LOG(INFO, true) << ", het_container_connect_processors, Connect NewData on " << i->getModuleName().c_str() << " port to " << k->getModuleName().c_str() << " on Process port\n";
              } else if (sigName == "NewData" && funName == "In1") {
                i->getOnNewDataGen()->template connect_member<processor>(k, &processor::setInput1);
                LOG(INFO, true) << ", het_container_connect_processors, Connect NewData on " << i->getModuleName().c_str() << " port to " << k->getModuleName().c_str() << " on Input1 port\n";
              } else if (sigName == "SetIn1" && funName == "Strt") {
                i->getFirstInputSet()->template connect_member<processor>(k, &processor::start);
                LOG(INFO, true) << ", het_container_connect_processors, Connect FirstInputSet on " << i->getModuleName().c_str() << " port to " << k->getModuleName().c_str() << " on Start port\n";
              } else {
                LOG(FATAL, true) << ", het_container_connect_processors, Undefined Connection from " << i->getModuleName().c_str() << " to " << k->getModuleName().c_str() << "\n";
              }
            }
          }
        }
      }
    }    
  }
};

struct het_container_run_sim : het_container_visitor_base<processor::sptr>
{
  template<class T>
  void operator()(std::vector<T>& _in)
  {
    for (auto const& i : _in) {
      if(i->getTrigStart()) {
        LOG(INFO, true) << ", het_container_run_sim, " << i->getModuleName().c_str() << "\n";
        i->start();
      }
    }    
  }
};

/*!
 * \brief Visitor pattern lambda function to print existing processor nodes in heterogeneous container.
 */
auto print_container = [](heterogeneous_container& _in){_in.visit_element(het_container_print_processor{}); std::cout << std::endl;};

/*!
 * \brief Visitor pattern lambda function to connect existing processor nodes in heterogeneous container to logger.
 */
auto connect_2_logger_container = [](heterogeneous_container& _in){_in.visit_element(het_container_connect_2_logger{}); std::cout << std::endl;};

/*!
 * \brief Visitor pattern lambda function to connect existing processor nodes in heterogeneous container together.
 */
auto connect_processors_container = [](heterogeneous_container& _in){_in.visit_elements(het_container_connect_processors{}); std::cout << std::endl;};

/*!
 * \brief Visitor pattern lambda function to find the starting processor node in heterogeneous container and start it.
 */
auto run_sim_container = [](heterogeneous_container& _in){_in.visit_elements(het_container_run_sim{}); std::cout << std::endl;};

/*!
 * \brief System builder class to construct the simulation pipeline out of JSON configuration file.
 */
class sys_builder : noncopyable
{
private:
  heterogeneous_container processors_;

public:

  /*!
   * \brief Constructor to create a simulation pipeline out of JSON configuration file.
   *
   * \param name        filename
   *
   */
  sys_builder(const char* cfg_file_name);

  virtual ~sys_builder() { processors_.clear(); }

  /*!
   * \brief print simulation pipeline.
   *
   * \param none
   */
  void print_pipeline();

  /*!
   * \brief get pipeline size.
   *
   * \param none
   */
  size_t get_pipeline_size();

  /*!
   * \brief get number of processors in pipeline.
   *
   * \param none
   */
  size_t get_pipeline_number_of_proc();

  /*!
   * \brief connect processors in pipeline to logger.
   *
   * \param none
   */
  void connect_pipeline_2_logger();

  /*!
   * \brief connect processors in pipeline together.
   *
   * \param none
   */
  void connect_pipeline_proc();

  /*!
   * \brief run simulation.
   *
   * \param none
   */
  void run_sim();
};

} // namespace pl_proc

#endif /* SYS_BUILDER_H */
