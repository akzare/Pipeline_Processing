/**
 * @file   vec_sink_blk.h
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   vec_sink_blk.h includes the implementation for the sink data class
 */

#ifndef VECTOR_SINK_H
#define VECTOR_SINK_H

#include "processor.h"

namespace pl_proc {

/*!
 * \brief T sink that writes to a vector
 */
template <class T>
class vec_sink_blk : public processor
{
private:
  pmt::pmt_t data_;
  std::vector<tag_t> tags_;


public:
  vec_sink_blk(ObjectIDModuleIndexType moduleIndex,
               const std::string& moduleName,
               const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,
               uint32_t noutput_items,
               bool trigStart);
  ~vec_sink_blk();

  void reset();
  pmt::pmt_t getData() const;
  std::vector<tag_t> getTags() const;
  void setInput1(pmt::pmt_t& input_items1) override { return; };
  void start() override { return; };
  bool getDone() override { return true; };
  void process(pmt::pmt_t& input_items) override;
};

} // namespace pl_proc

#endif /* VECTOR_SINK_H */
