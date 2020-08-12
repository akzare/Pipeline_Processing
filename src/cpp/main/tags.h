/**
 * @file   tags.h
 *
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 * Originally, this file was part of GNU Radio
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   tags.h includes the implementation for the TAG generator class
 */

#ifndef TAGS_H
#define TAGS_H

#include "pmt.h"
#include "id.h"


namespace pl_proc {

/*!
 * \brief Implementation of TAG generator
 */
struct tag_t {
  //! the item \p tag occurred at (as a uint64_t)
  int64_t timetag_;

  //! the key of \p tag (as a PMT symbol)
  ObjectID key_;

  //! the value of \p tag (as a PMT)
  pmt::DataType valueDataType_;
  pmt::pmt_t value_;

  /*!
   * Comparison function to test which tag, \p x or \p y, came
   * first in time
   */
  static inline bool timetag_compare(const tag_t& x, const tag_t& y)
  {
    return x.timetag_ < y.timetag_;
  }

  inline bool operator==(const tag_t& t) const
  {
    return (t.key_ == key_) &&
           (t.value_ == value_) &&
           (t.key_.GetModuleType() == key_.GetModuleType()) &&
           (t.key_.GetModuleIndex() == key_.GetModuleIndex()) &&
           (t.key_.GetPaketIndex() == key_.GetPaketIndex()) &&
           (t.timetag_ == timetag_);
  }

  tag_t(int64_t timetag, ObjectID key, pmt::DataType valueDataType, pmt::pmt_t value)
        : timetag_(timetag),
          key_(key),
          valueDataType_(valueDataType),
          value_(value)
  {
  }

  tag_t(const tag_t& rhs)
        : timetag_(rhs.timetag_), key_(rhs.key_), valueDataType_(rhs.valueDataType_), value_(rhs.value_)
  {
  }

  tag_t& operator=(const tag_t& rhs)
  {
    if (this != &rhs) {
      timetag_ = rhs.timetag_;
      key_ = rhs.key_;
      valueDataType_ = rhs.valueDataType_;
      value_ = rhs.value_;
    }
    return (*this);
  }

  ~tag_t() {}
};

} // namespace pl_proc

#endif /*TAGS_H*/
