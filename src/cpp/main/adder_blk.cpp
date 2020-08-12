/**
 * @file   adder_blk.cpp
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   adder_blk.cpp includes the implementation for the adder class
 */


#include "adder_blk.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>


namespace pl_proc {


template <class T>
adder_blk<T>::adder_blk(ObjectIDModuleIndexType moduleIndex,
                        const std::string& moduleName,
                        const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,
                        uint32_t noutput_items,
                        bool trigStart)
  : processor(static_cast<ObjectIDModuleType>(ModuleType::ADDER_MODULE), 
              moduleIndex, 
              moduleName, 
              adjacencyConnection, 
              noutput_items,
              trigStart)
{
  output_items_ = pmt::make_genVector<T>(noutput_items_, 0);
}

template <class T>
void adder_blk<T>::setInput1(pmt::pmt_t& input_items1)
{
  std::lock_guard<std::mutex> locker(mutex_);

  assert(pmt::getLength_genVector<T>(input_items1) == noutput_items_);

  input_items1_ = input_items1;
  pmt::genVector_fill<T>(output_items_, 0);
  emitFirstInput();
}

template <class T>
void adder_blk<T>::process(pmt::pmt_t& input_items2)
{
//  std::lock_guard<std::mutex> locker(mutex_);
  assert(pmt::getLength_genVector<T>(output_items_) == pmt::getLength_genVector<T>(input_items2));

  const T* inVec1 = pmt::genVector_raw<T>(input_items1_);
  const T* inVec2 = pmt::genVector_raw<T>(input_items2);
  T* outVec = pmt::genVector_writable_raw<T>(output_items_);

  std::transform(inVec1, inVec1+pmt::getLength_genVector<T>(input_items1_),
                 inVec2,
                 outVec, std::plus<T>());

  emitNewTag(pmt::getType_genVector<T>(output_items_));
  emitNewData();
}


template class adder_blk<std::int8_t>;
template class adder_blk<std::uint8_t>;
template class adder_blk<std::int16_t>;
template class adder_blk<std::uint16_t>;
template class adder_blk<std::int32_t>;
template class adder_blk<std::uint32_t>;
template class adder_blk<std::int64_t>;
template class adder_blk<std::uint64_t>;
template class adder_blk<std::complex<float>>;
template class adder_blk<float>;

} // namespace pl_proc
