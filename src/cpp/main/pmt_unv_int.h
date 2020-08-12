/* -*- c++ -*- */
/*
 * Copyright 2006,2009,2018 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  It has been modified & adapted to the Pipeline Processing Framework with
 *  adding pmt_genvector by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 *
 */

#ifndef INCLUDED_PMT_UNV_INT_H
#define INCLUDED_PMT_UNV_INT_H

#include "pmt_int.h"

#include <cstdint>
#include <vector>


namespace pl_proc {

namespace pmt {
////////////////////////////////////////////////////////////////////////////
//                           pmt_genvector
////////////////////////////////////////////////////////////////////////////
template <class T>
class pmt_genVector : public pmt_uniform_vector
{
  std::vector<T> d_v;

public:
  pmt_genVector(size_t k);
  pmt_genVector(size_t k, T fill);
  pmt_genVector(size_t k, const T* data);

  DataType check_type() const;
  bool is_genVector() const { return true; }
  size_t length() const { return d_v.size(); }
  size_t itemsize() const { return sizeof(T); }
  T ref(size_t k) const;
  void fill(T x);
  void set(size_t k, T x);
  const T* elements(size_t& len);
  T* writable_elements(size_t& len);
  const void* uniform_elements(size_t& len);
  void* uniform_writable_elements(size_t& len);
  virtual const std::string string_ref(size_t k) const;
};

} /* namespace pmt */

} // namespace pl_proc

#endif
