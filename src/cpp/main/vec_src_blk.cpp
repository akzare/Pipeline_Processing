/**
 * @file   vec_src_blk.cpp
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   vec_src_blk.cpp includes the implementation for the source data generator class
 */


#include "vec_src_blk.h"
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace pl_proc {


template <class T>
vec_src_blk<T>::vec_src_blk(ObjectIDModuleIndexType moduleIndex,
                            const std::string& moduleName,
                            const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,
                            uint32_t noutput_items,
                            bool trigStart,
                            const pmt::pmt_t& data,
                            bool repeat,
                            unsigned int vlen)
  : processor(static_cast<ObjectIDModuleType>(ModuleType::SRC_VEC_MODULE), 
              moduleIndex, 
              moduleName, 
              adjacencyConnection, 
              noutput_items,
              trigStart),
    data_(data),
    repeat_(repeat),
    offset_(0),
    vlen_(vlen),
    done_(false)
{
  if (!pmt::is_genVector<T>(data))
    throw std::invalid_argument("pmt data must be generic vector (genVector)");

  dataType_= pmt::getType_genVector<T>(data);

  output_items_ = pmt::make_genVector<T>(noutput_items_);

  if ((pmt::getLength_genVector<T>(data) % vlen) != 0)
    throw std::invalid_argument("data length must be a multiple of vlen");
}

template <class T>
vec_src_blk<T>::~vec_src_blk()
{
}

template <class T>
void vec_src_blk<T>::setData(const pmt::pmt_t& data)
{
  data_ = data;
  rewind();
}

template <class T>
void vec_src_blk<T>::start()
{
  size_t len = 0;
  const T* inVec = pmt::genVector_elements<T>(data_, len);
  T* outVec = pmt::genVector_writable_elements<T>(output_items_, len);

  if (repeat_) {
    unsigned int size = pmt::getLength_genVector<T>(data_);
    unsigned int offset = offset_;
    if (size == 0) {
  	  done_ = true;
      return;
    }

    for (int i = 0; i < static_cast<int>(noutput_items_ * vlen_); i++) {
   	  outVec[i] = inVec[offset++];
      if (offset >= size) {
        offset = 0;
      }
    }

    offset_ = offset;
    emitNewTag(dataType_);
    emitNewData();
    return;
  } else {
    if (offset_ >= pmt::getLength_genVector<T>(data_)) {
      done_ =  true;
      return; // Done!
    }

    unsigned n = std::min((unsigned)pmt::getLength_genVector<T>(data_) - offset_,
                          (unsigned)noutput_items_);
    for (unsigned i = 0; i < n; i++) {
   	  outVec[i] = inVec[offset_ + i];
    }
    offset_ += n;

    emitNewTag(dataType_);
    emitNewData();
    return;
  }
}

template class vec_src_blk<std::uint8_t>;
template class vec_src_blk<std::int8_t>;
template class vec_src_blk<std::uint16_t>;
template class vec_src_blk<std::int16_t>;
template class vec_src_blk<std::uint32_t>;
template class vec_src_blk<std::int32_t>;
template class vec_src_blk<std::uint64_t>;
template class vec_src_blk<std::int64_t>;
template class vec_src_blk<float>;
template class vec_src_blk<double>;
template class vec_src_blk<std::complex<float>>;
template class vec_src_blk<std::complex<double>>;


} // namespace pl_proc
