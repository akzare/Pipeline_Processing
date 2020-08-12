/**
 * @file   adder_blk.h
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   adder_blk.h includes the implementation for the adder class
 */


#ifndef ADD_BLK_IMPL_H
#define ADD_BLK_IMPL_H

#include "processor.h"
#include <vector>
#include <mutex>


namespace pl_proc {

template <class T>
class adder_blk : public processor
{
private:

public:
  adder_blk(ObjectIDModuleIndexType moduleIndex,
            const std::string& moduleName,
            const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,
            uint32_t noutput_items,
            bool trigStart);
  virtual ~adder_blk() { };
  void setInput1(pmt::pmt_t& input_items1) override;
  void start() override { return; };
  bool getDone() override { return true; };  
  void process(pmt::pmt_t& input_items2) override;
};

} // namespace pl_proc

#endif /* ADD_BLK_IMPL_H */
