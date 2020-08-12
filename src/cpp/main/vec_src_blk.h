/**
 * @file   vec_src_blk.h
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   vec_src_blk.h includes the implementation for the source data generator class
 */


#ifndef VECTOR_SOURCE_H
#define VECTOR_SOURCE_H

#include "processor.h"

namespace pl_proc {

/*!
 * \brief Source that streams T items based on the input \p data vector.
 *
 * \details
 * This block produces a stream of samples based on an input
 * vector. The data can repeat infinitely
 * until the flowgraph is terminated by some other event or, the
 * default, run the data once and stop.
 */
template <class T>
class vec_src_blk : public processor
{
private:
  pmt::DataType dataType_;
  pmt::pmt_t data_;
  bool repeat_;
  unsigned int offset_;
  unsigned int vlen_;
  bool done_;

public:
  vec_src_blk(ObjectIDModuleIndexType moduleIndex,
              const std::string& moduleName,
              const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,
              uint32_t noutput_items,
              bool trigStart,
              const pmt::pmt_t& data,
              bool repeat,
              unsigned int vlen);
  ~vec_src_blk();

  void rewind() { offset_ = 0; }
  void setData(const pmt::pmt_t& data);
  void setRepeat(bool repeat) { repeat_ = repeat; };
  void setInput1(pmt::pmt_t& input_items1) override { return; };  
  void start() override;  
  bool getDone() override { return done_; };
  void process(pmt::pmt_t& input_items) override  { return; };
  pmt::DataType getDataType() { return dataType_; }
};

} // namespace pl_proc

#endif /* VECTOR_SOURCE_H */
