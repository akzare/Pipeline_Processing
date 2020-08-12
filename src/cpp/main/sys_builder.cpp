/**
 * @file   sys_builder.cpp
 *
 * @author Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *         Eric J. Mayo eric@pozicom.net
 *
 * @brief   sys_builder.cpp includes the system builder for the Pipeline Processing Framework.
 */


#include "sys_builder.h"
#include "util.h"
#include "logging.h"
#include "id.h"
#include "json11.h"

#include "processor_factory.h"

#include "pmt.h"
#include "tags.h"


#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <regex>

namespace pl_proc {

sys_builder::sys_builder(const char* cfg_file_name)
{
  // read json configuration file
  std::ifstream t(cfg_file_name);
  std::stringstream cfgFileBuff;
  cfgFileBuff << t.rdbuf();
  t.close();

  // parse json configuration file
  std::string err;
  const auto json = json11::Json::parse(cfgFileBuff.str(), err);
  CHECK(err.empty()) << "Json::parse failed with errors: " << err;

  // print simulation information details (json __sim_model_info__ field) into logger
  for (auto &k : json["__sim_model_info__"].object_items()) {
    if (k.first == "__name__")   LOG(INFO, true) << ", sys_builder, Sim Model Name: " << k.second.dump() <<"\n";
    if (k.first == "__date__")   LOG(INFO, true) << ", sys_builder, Sim Model Date: " << k.second.dump() <<"\n";
    if (k.first == "__author__") LOG(INFO, true) << ", sys_builder, Sim Model Author: " << k.second.dump() <<"\n";
    if (k.first == "__rev__")    LOG(INFO, true) << ", sys_builder, Sim Model Revision: " << k.second.dump() <<"\n";
    if (k.first == "__desc__")   LOG(INFO, true) << ", sys_builder, Sim Model Description: " << k.second.dump() <<"\n";
  }

  int pkt_len = 0;
  int nb_pkt = 0;
  std::string data_file_name;

  // print simulation information details (json __general__ field) into logger
  for (auto &k : json["__general__"].object_items()) {
    if (k.first == "__paket_len__") {
      LOG(INFO, true) << ", sys_builder, Packet Length: "     << k.second.int_value() <<"\n";
      pkt_len = k.second.int_value();
    }
    if (k.first == "__num_of_paket__") { 
      LOG(INFO, true) << ", sys_builder, Number of Packets: " << k.second.int_value() <<"\n";
      nb_pkt = k.second.int_value();
    }
    if (k.first == "__data_file_name__") {
      LOG(INFO, true) << ", sys_builder, Data File Name: "    << k.second.string_value() <<"\n";
      data_file_name = k.second.string_value();
    }
  }

  std::vector<std::uint8_t> vec_src;
  if (!is_file_exist(data_file_name.c_str())) {
    vec_src = genrandvec<std::uint8_t>(0, 1, nb_pkt*pkt_len);

    // open the file
    std::ofstream fout(data_file_name, std::ios::out | std::ios::binary);
    fout.write(reinterpret_cast<const char*>(&vec_src[0]), vec_src.size()*sizeof(std::uint8_t));
    fout.close();
  }
  else {
    // open the file
    std::ifstream fin(data_file_name, std::ios::in | std::ios::binary);

    // get its size
    std::streampos fileSize;

    fin.seekg(0, std::ios::end);
    fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);

    // reserve capacity
    vec_src.reserve(fileSize);

    std::copy(std::istream_iterator<std::uint8_t>(fin),
              std::istream_iterator<std::uint8_t>(),
              std::back_inserter(vec_src));
  }

  pmt::pmt_t pmtVecSrc = pmt::init_genVector<std::uint8_t>(vec_src.size(), vec_src);

  // iterate over processors nodes print (json __processors__ field) and insert them into heterogeneous container
  ObjectIDModuleIndexType idx = 0;
  LOG(INFO, true) << ", sys_builder, Sim Model Processor Nodes: " << "\n";
  for (auto &k : json["__processors__"].object_items()) {
    idx++;
    LOG(INFO, true) << "  - idx(" << unsigned(idx) << ") - " << k.first << ": " << "\n";
    LOG(INFO, true) << "    - Processor Type: " << k.second["__proc_type__"].string_value() << "\n";
    LOG(INFO, true) << "    - Input Data Type: " << k.second["__in_data_type__"].string_value() << "\n";
    LOG(INFO, true) << "    - Output Data Type: " << k.second["__out_data_type__"].string_value() << "\n";
    LOG(INFO, true) << "    - Output Vector Size: " << k.second["__out_vector_size__"].int_value() << "\n";
    LOG(INFO, true) << "    - Trigger Start: " << k.second["__trig_start__"].bool_value() << "\n";

    std::list<std::tuple<std::string, std::string, std::string>> conList;
    for (auto &l : k.second["__adjacency_connection_to__"].object_items()) {
      conList.emplace_back(std::make_tuple(l.second[0].string_value(), 
                                           l.second[1].string_value(), 
                                           l.second[2].string_value()));
    }

    // Create Random Vector Generator node
    if (k.second["__proc_type__"].string_value() == "RAND_VEC_GEN")
    {
    }
    // create bits source node
    else if (k.second["__proc_type__"].string_value() == "SRC_VEC_PROC")
    {  
      processor::sptr bitsSrcNode = proc_factory::createSRC(k.second["__out_data_type__"].string_value(), 
                                                            idx, 
                                                            k.first, 
                                                            std::move(conList), 
                                                            k.second["__out_vector_size__"].int_value(), 
                                                            k.second["__trig_start__"].bool_value(), 
                                                            pmtVecSrc, 
                                                            k.second["__repeat__"].bool_value(), 
                                                            k.second["__vlen__"].int_value());        
      processors_.push_back(std::move(bitsSrcNode));
    }
    // create adder node
    else if (k.second["__proc_type__"].string_value() == "ADDER_PROC")
    {
      processor::sptr adderNode = proc_factory::createADDER(k.second["__out_data_type__"].string_value(),
                                                            idx, 
                                                            k.first, 
                                                            std::move(conList), 
                                                            k.second["__out_vector_size__"].int_value(),
                                                            k.second["__trig_start__"].bool_value());
      processors_.push_back(std::move(adderNode));
    }
    // create vector sink node
    else if (k.second["__proc_type__"].string_value() == "SINK_VEC_PROC")
    {
      processor::sptr sinkNode = proc_factory::createSINK(k.second["__in_data_type__"].string_value(), 
                                                          idx, 
                                                          k.first, 
                                                          std::move(conList), 
                                                          k.second["__out_vector_size__"].int_value(), 
                                                          k.second["__trig_start__"].bool_value());
      processors_.push_back(std::move(sinkNode));
    }
    // Logger node
    else if (k.second["__proc_type__"].string_value() == "LOGGER")
    {
    }
    else
    {
      LOG(ERROR, true) << "ModuleType is unknown";
    }
  }

}

void sys_builder::print_pipeline()
{
  print_container(processors_);
}

size_t sys_builder::get_pipeline_size()
{
  return processors_.size();
}

size_t sys_builder::get_pipeline_number_of_proc()
{
  return processors_.number_of<processor::sptr>();
}

void sys_builder::connect_pipeline_2_logger()
{
  connect_2_logger_container(processors_);
}

void sys_builder::connect_pipeline_proc()
{
  connect_processors_container(processors_);
}

void sys_builder::run_sim()
{
  run_sim_container(processors_);
}

} // namespace pl_proc
