/**
 * @file   vec_sink_blk.cpp
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   vec_sink_blk.cpp includes the implementation for the sink data class
 */

#include "vec_sink_blk.h"
#include <algorithm>
#include <stdexcept>


namespace pl_proc {

  template <class T>
  vec_sink_blk<T>::vec_sink_blk(ObjectIDModuleIndexType moduleIndex,
                                const std::string& moduleName,
                                const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,
                                uint32_t noutput_items,
                                bool trigStart)
    : processor(static_cast<ObjectIDModuleType>(ModuleType::SINK_VEC_MODULE), 
                moduleIndex,
                moduleName,
                adjacencyConnection,
                noutput_items,
                trigStart)
  {
    data_ = pmt::make_genVector<T>(noutput_items, 0);
  }

  template <class T>
  vec_sink_blk<T>::~vec_sink_blk()
  {
  }

  template <class T>
  pmt::pmt_t vec_sink_blk<T>::getData() const
  {
//    std::lock_guard<std::mutex> locker(mutex_);
    return data_;
  }

  template <class T>
  std::vector<tag_t> vec_sink_blk<T>::getTags() const
  {
//    std::lock_guard<std::mutex> locker(mutex_);
    return tags_;
  }

  template <class T>
  void vec_sink_blk<T>::reset()
  {
    std::lock_guard<std::mutex> locker(mutex_);
    tags_.clear();
    pmt::genVector_fill<T>(data_, 0);
  }

  template <class T>
  void vec_sink_blk<T>::process(pmt::pmt_t& input_items)
  {
    const T* inVec = pmt::genVector_raw<T>(input_items);

    // can't touch this (as long as process() is working, the accessors shall not
    // read the data
    std::lock_guard<std::mutex> locker(mutex_);
//    for (unsigned int i = 0; i < noutput_items_; i++)
//      data_.push_back(iptr[i]);
  }

  template class vec_sink_blk<std::int8_t>;
  template class vec_sink_blk<std::uint8_t>;
  template class vec_sink_blk<std::int16_t>;
  template class vec_sink_blk<std::uint16_t>;
  template class vec_sink_blk<std::int32_t>;
  template class vec_sink_blk<std::uint32_t>;
  template class vec_sink_blk<std::int64_t>;
  template class vec_sink_blk<std::uint64_t>;
  template class vec_sink_blk<float>;
  template class vec_sink_blk<std::complex<float>>;

} // namespace pl_proc
