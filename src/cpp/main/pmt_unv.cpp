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
 */

// This file is machine generated using the generate_unv.py tool


#include "pmt_int.h"
#include "pmt_unv_int.h"
#include "pmt.h"
#include <vector>
#include <cstring>
#include <type_traits>


namespace pl_proc {

namespace pmt {

template <typename T>
T lexical_cast(const std::string& str)
{
  T var;
  std::istringstream iss;
  iss.str(str);
  iss >> var;
  // deal with any error bits that may have been set on the stream
  return var;
}

////////////////////////////////////////////////////////////////////////////
//                           pmt_genvector
////////////////////////////////////////////////////////////////////////////

template <class T> static pmt_genVector<T>* _genVector(pmt_t x) { return dynamic_cast<pmt_genVector<T>*>(x.get()); }

template class pmt_genVector<uint8_t>;
template class pmt_genVector<int8_t>;
template class pmt_genVector<uint16_t>;
template class pmt_genVector<int16_t>;
template class pmt_genVector<uint32_t>;
template class pmt_genVector<int32_t>;
template class pmt_genVector<uint64_t>;
template class pmt_genVector<int64_t>;
template class pmt_genVector<float>;
template class pmt_genVector<double>;
template class pmt_genVector<std::complex<float>>;
template class pmt_genVector<std::complex<double>>;

template <class T>
pmt_genVector<T>::pmt_genVector(size_t k) : d_v(k)
{
}

template <class T>
pmt_genVector<T>::pmt_genVector(size_t k, T fill) : d_v(k)
{
  for (size_t i = 0; i < k; i++)
    d_v[i] = fill;
}

template <class T>
pmt_genVector<T>::pmt_genVector(size_t k, const T* data) : d_v(k)
{
  if (k)
    std::memcpy(&d_v[0], data, k * sizeof(T));
}

template <class T>
T pmt_genVector<T>::ref(size_t k) const
{
  if (k >= length())
    throw out_of_range("pmt_genVector_ref", from_long(k));
  return d_v[k];
}

template <class T>
void pmt_genVector<T>::fill(T x)
{
  std::fill(d_v.begin(), d_v.end(), x);
}

template <class T>
void pmt_genVector<T>::set(size_t k, T x)
{
  if (k >= length())
    throw out_of_range("pmt_genVector_set", from_long(k));
  d_v[k] = x;
}

template <class T>
const T* pmt_genVector<T>::elements(size_t& len)
{
  len = length();
  return len ? &d_v[0] : nullptr;
}

template <class T>
T* pmt_genVector<T>::writable_elements(size_t& len)
{
  len = length();
  return len ? &d_v[0] : nullptr;
}

template <class T>
const void* pmt_genVector<T>::uniform_elements(size_t& len)
{
  len = length() * sizeof(T);
  return len ? &d_v[0] : nullptr;
}

template <class T>
void* pmt_genVector<T>::uniform_writable_elements(size_t& len)
{
  len = length() * sizeof(T);
  return len ? (&d_v[0]) : nullptr;
}

template <class T>
const std::string pmt_genVector<T>::string_ref(size_t k) const
{
  std::string d;
  return d;//std::to_string(ref(k));
}

template <class T>
DataType pmt_genVector<T>::check_type() const
{
  if (std::is_same<T, uint8_t>::value) { return DataType::GVEC_UINT8; }
  if (std::is_same<T, int8_t>::value) { return DataType::GVEC_INT8; }
  if (std::is_same<T, uint16_t>::value) { return DataType::GVEC_UINT16; }
  if (std::is_same<T, int16_t>::value) { return DataType::GVEC_INT16; }
  if (std::is_same<T, uint32_t>::value) { return DataType::GVEC_INT32; }
  if (std::is_same<T, int32_t>::value) { return DataType::GVEC_UINT32; }
  if (std::is_same<T, uint64_t>::value) { return DataType::GVEC_INT64; }
  if (std::is_same<T, int64_t>::value) { return DataType::GVEC_UINT64; }
  if (std::is_same<T, float>::value) { return DataType::GVEC_FLOAT; }
  if (std::is_same<T, double>::value) { return DataType::GVEC_DOUBLE; }
  if (std::is_same<T, std::complex<float>>::value) { return DataType::GVEC_COMPLEX_FLOAT; }
  if (std::is_same<T, std::complex<double>>::value) { return DataType::GVEC_COMPLEX_DOUBLE; }

  return DataType::UNKNOWN;
}


template <class T>
bool is_genVector(pmt_t obj) { return _genVector<T>(obj)->is_genVector(); }

template bool is_genVector<uint8_t>(pmt_t obj);
template bool is_genVector<int8_t>(pmt_t obj);
template bool is_genVector<uint16_t>(pmt_t obj);
template bool is_genVector<int16_t>(pmt_t obj);
template bool is_genVector<uint32_t>(pmt_t obj);
template bool is_genVector<int32_t>(pmt_t obj);
template bool is_genVector<uint64_t>(pmt_t obj);
template bool is_genVector<int64_t>(pmt_t obj);
template bool is_genVector<float>(pmt_t obj);
template bool is_genVector<double>(pmt_t obj);
template bool is_genVector<std::complex<float>>(pmt_t obj);
template bool is_genVector<std::complex<double>>(pmt_t obj);



template <class T>
size_t getLength_genVector(pmt_t obj) { return _genVector<T>(obj)->length(); }

template size_t getLength_genVector<uint8_t>(pmt_t obj);
template size_t getLength_genVector<int8_t>(pmt_t obj);
template size_t getLength_genVector<uint16_t>(pmt_t obj);
template size_t getLength_genVector<int16_t>(pmt_t obj);
template size_t getLength_genVector<uint32_t>(pmt_t obj);
template size_t getLength_genVector<int32_t>(pmt_t obj);
template size_t getLength_genVector<uint64_t>(pmt_t obj);
template size_t getLength_genVector<int64_t>(pmt_t obj);
template size_t getLength_genVector<float>(pmt_t obj);
template size_t getLength_genVector<double>(pmt_t obj);
template size_t getLength_genVector<std::complex<float>>(pmt_t obj);
template size_t getLength_genVector<std::complex<double>>(pmt_t obj);



template <class T>
size_t getItemSize_genVector(pmt_t obj) { return _genVector<T>(obj)->itemsize(); }

template size_t getItemSize_genVector<uint8_t>(pmt_t obj);
template size_t getItemSize_genVector<int8_t>(pmt_t obj);
template size_t getItemSize_genVector<uint16_t>(pmt_t obj);
template size_t getItemSize_genVector<int16_t>(pmt_t obj);
template size_t getItemSize_genVector<uint32_t>(pmt_t obj);
template size_t getItemSize_genVector<int32_t>(pmt_t obj);
template size_t getItemSize_genVector<uint64_t>(pmt_t obj);
template size_t getItemSize_genVector<int64_t>(pmt_t obj);
template size_t getItemSize_genVector<float>(pmt_t obj);
template size_t getItemSize_genVector<double>(pmt_t obj);
template size_t getItemSize_genVector<std::complex<float>>(pmt_t obj);
template size_t getItemSize_genVector<std::complex<double>>(pmt_t obj);



template <class T>
DataType getType_genVector(pmt_t obj) { return _genVector<T>(obj)->check_type(); }

template DataType getType_genVector<uint8_t>(pmt_t obj);
template DataType getType_genVector<int8_t>(pmt_t obj);
template DataType getType_genVector<uint16_t>(pmt_t obj);
template DataType getType_genVector<int16_t>(pmt_t obj);
template DataType getType_genVector<uint32_t>(pmt_t obj);
template DataType getType_genVector<int32_t>(pmt_t obj);
template DataType getType_genVector<uint64_t>(pmt_t obj);
template DataType getType_genVector<int64_t>(pmt_t obj);
template DataType getType_genVector<float>(pmt_t obj);
template DataType getType_genVector<double>(pmt_t obj);
template DataType getType_genVector<std::complex<float>>(pmt_t obj);
template DataType getType_genVector<std::complex<double>>(pmt_t obj);



template <class T>
pmt_t make_genVector(size_t k, T fill) { return pmt_t(new pmt_genVector<T>(k, fill)); }

template pmt_t make_genVector<uint8_t>(size_t k, uint8_t fill);
template pmt_t make_genVector<int8_t>(size_t k, int8_t fill);
template pmt_t make_genVector<uint16_t>(size_t k, uint16_t fill);
template pmt_t make_genVector<int16_t>(size_t k, int16_t fill);
template pmt_t make_genVector<uint32_t>(size_t k, uint32_t fill);
template pmt_t make_genVector<int32_t>(size_t k, int32_t fill);
template pmt_t make_genVector<uint64_t>(size_t k, uint64_t fill);
template pmt_t make_genVector<int64_t>(size_t k, int64_t fill);
template pmt_t make_genVector<float>(size_t k, float fill);
template pmt_t make_genVector<double>(size_t k, double fill);
template pmt_t make_genVector<std::complex<float>>(size_t k, std::complex<float> fill);
template pmt_t make_genVector<std::complex<double>>(size_t k, std::complex<double> fill);



template <class T>
pmt_t make_genVector(size_t k) { return pmt_t(new pmt_genVector<T>(k)); }

template pmt_t make_genVector<uint8_t>(size_t k);
template pmt_t make_genVector<int8_t>(size_t k);
template pmt_t make_genVector<uint16_t>(size_t k);
template pmt_t make_genVector<int16_t>(size_t k);
template pmt_t make_genVector<uint32_t>(size_t k);
template pmt_t make_genVector<int32_t>(size_t k);
template pmt_t make_genVector<uint64_t>(size_t k);
template pmt_t make_genVector<int64_t>(size_t k);
template pmt_t make_genVector<float>(size_t k);
template pmt_t make_genVector<double>(size_t k);
template pmt_t make_genVector<std::complex<float>>(size_t k);
template pmt_t make_genVector<std::complex<double>>(size_t k);



template <class T>
pmt_t init_genVector(size_t k, const T* data)
{
  return pmt_t(new pmt_genVector<T>(k, data));
}

template pmt_t init_genVector<uint8_t>(size_t k, const uint8_t* data);
template pmt_t init_genVector<int8_t>(size_t k, const int8_t* data);
template pmt_t init_genVector<uint16_t>(size_t k, const uint16_t* data);
template pmt_t init_genVector<int16_t>(size_t k, const int16_t* data);
template pmt_t init_genVector<uint32_t>(size_t k, const uint32_t* data);
template pmt_t init_genVector<int32_t>(size_t k, const int32_t* data);
template pmt_t init_genVector<uint64_t>(size_t k, const uint64_t* data);
template pmt_t init_genVector<int64_t>(size_t k, const int64_t* data);
template pmt_t init_genVector<float>(size_t k, const float* data);
template pmt_t init_genVector<double>(size_t k, const double* data);
template pmt_t init_genVector<std::complex<float>>(size_t k, const std::complex<float>* data);
template pmt_t init_genVector<std::complex<double>>(size_t k, const std::complex<double>* data);



template <class T>
pmt_t init_genVector(size_t k, const std::vector<T>& data)
{
  if (k) {
    return pmt_t(new pmt_genVector<T>(k, &data[0]));
  }
  return pmt_t(
        new pmt_genVector<T>(k, static_cast<T>(0))); // fills an empty vector with 0
}

template pmt_t init_genVector<uint8_t>(size_t k, const std::vector<uint8_t>& data);
template pmt_t init_genVector<int8_t>(size_t k, const std::vector<int8_t>& data);
template pmt_t init_genVector<uint16_t>(size_t k, const std::vector<uint16_t>& data);
template pmt_t init_genVector<int16_t>(size_t k, const std::vector<int16_t>& data);
template pmt_t init_genVector<uint32_t>(size_t k, const std::vector<uint32_t>& data);
template pmt_t init_genVector<int32_t>(size_t k, const std::vector<int32_t>& data);
template pmt_t init_genVector<uint64_t>(size_t k, const std::vector<uint64_t>& data);
template pmt_t init_genVector<int64_t>(size_t k, const std::vector<int64_t>& data);
template pmt_t init_genVector<float>(size_t k, const std::vector<float>& data);
template pmt_t init_genVector<double>(size_t k, const std::vector<double>& data);
template pmt_t init_genVector<std::complex<float>>(size_t k, const std::vector<std::complex<float>>& data);
template pmt_t init_genVector<std::complex<double>>(size_t k, const std::vector<std::complex<double>>& data);



template <class T>
T genVector_ref(pmt_t vector, size_t k)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_ref", vector);
  return _genVector<T>(vector)->ref(k);
}

template uint8_t genVector_ref<uint8_t>(pmt_t vector, size_t k);
template int8_t genVector_ref<int8_t>(pmt_t vector, size_t k);
template uint16_t genVector_ref<uint16_t>(pmt_t vector, size_t k);
template int16_t genVector_ref<int16_t>(pmt_t vector, size_t k);
template uint32_t genVector_ref<uint32_t>(pmt_t vector, size_t k);
template int32_t genVector_ref<int32_t>(pmt_t vector, size_t k);
template uint64_t genVector_ref<uint64_t>(pmt_t vector, size_t k);
template int64_t genVector_ref<int64_t>(pmt_t vector, size_t k);
template float genVector_ref<float>(pmt_t vector, size_t k);
template double genVector_ref<double>(pmt_t vector, size_t k);
template std::complex<float> genVector_ref<std::complex<float>>(pmt_t vector, size_t k);
template std::complex<double> genVector_ref<std::complex<double>>(pmt_t vector, size_t k);



template <class T>
void genVector_set(pmt_t vector, size_t k, T obj)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_set", vector);
  _genVector<T>(vector)->set(k, obj);
}

template void genVector_set<uint8_t>(pmt_t vector, size_t k, uint8_t obj);
template void genVector_set<int8_t>(pmt_t vector, size_t k, int8_t obj);
template void genVector_set<uint16_t>(pmt_t vector, size_t k, uint16_t obj);
template void genVector_set<int16_t>(pmt_t vector, size_t k, int16_t obj);
template void genVector_set<uint32_t>(pmt_t vector, size_t k, uint32_t obj);
template void genVector_set<int32_t>(pmt_t vector, size_t k, int32_t obj);
template void genVector_set<uint64_t>(pmt_t vector, size_t k, uint64_t obj);
template void genVector_set<int64_t>(pmt_t vector, size_t k, int64_t obj);
template void genVector_set<float>(pmt_t vector, size_t k, float obj);
template void genVector_set<double>(pmt_t vector, size_t k, double obj);
template void genVector_set<std::complex<float>>(pmt_t vector, size_t k, std::complex<float> obj);
template void genVector_set<std::complex<double>>(pmt_t vector, size_t k, std::complex<double> obj);



template <class T>
void genVector_fill(pmt_t vector, T obj)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_set", vector);
  _genVector<T>(vector)->fill(obj);
}

template void genVector_fill<uint8_t>(pmt_t vector, uint8_t obj);
template void genVector_fill<int8_t>(pmt_t vector, int8_t obj);
template void genVector_fill<uint16_t>(pmt_t vector, uint16_t obj);
template void genVector_fill<int16_t>(pmt_t vector, int16_t obj);
template void genVector_fill<uint32_t>(pmt_t vector, uint32_t obj);
template void genVector_fill<int32_t>(pmt_t vector, int32_t obj);
template void genVector_fill<uint64_t>(pmt_t vector, uint64_t obj);
template void genVector_fill<int64_t>(pmt_t vector, int64_t obj);
template void genVector_fill<float>(pmt_t vector, float obj);
template void genVector_fill<double>(pmt_t vector, double obj);
template void genVector_fill<std::complex<float>>(pmt_t vector, std::complex<float> obj);
template void genVector_fill<std::complex<double>>(pmt_t vector, std::complex<double> obj);



template <class T>
const void* genVector_uniform_elements(pmt_t vector, size_t& len)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_elements", vector);
  return _genVector<T>(vector)->uniform_elements(len);
}

template const void* genVector_uniform_elements<uint8_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<int8_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<uint16_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<int16_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<uint32_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<int32_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<uint64_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<int64_t>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<float>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<double>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<std::complex<float>>(pmt_t vector, size_t& len);
template const void* genVector_uniform_elements<std::complex<double>>(pmt_t vector, size_t& len);



template <class T>
void* genVector_uniform_writable_elements(pmt_t vector, size_t& len)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_elements", vector);
  return _genVector<T>(vector)->uniform_writable_elements(len);
}

template void* genVector_uniform_writable_elements<uint8_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<int8_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<uint16_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<int16_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<uint32_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<int32_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<uint64_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<int64_t>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<float>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<double>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<std::complex<float>>(pmt_t vector, size_t& len);
template void* genVector_uniform_writable_elements<std::complex<double>>(pmt_t vector, size_t& len);



template <class T>
const T* genVector_elements(pmt_t vector, size_t& len)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_elements", vector);
  return _genVector<T>(vector)->elements(len);
}

template const uint8_t* genVector_elements<uint8_t>(pmt_t vector, size_t& len);
template const int8_t* genVector_elements<int8_t>(pmt_t vector, size_t& len);
template const uint16_t* genVector_elements<uint16_t>(pmt_t vector, size_t& len);
template const int16_t* genVector_elements<int16_t>(pmt_t vector, size_t& len);
template const uint32_t* genVector_elements<uint32_t>(pmt_t vector, size_t& len);
template const int32_t* genVector_elements<int32_t>(pmt_t vector, size_t& len);
template const uint64_t* genVector_elements<uint64_t>(pmt_t vector, size_t& len);
template const int64_t* genVector_elements<int64_t>(pmt_t vector, size_t& len);
template const float* genVector_elements<float>(pmt_t vector, size_t& len);
template const double* genVector_elements<double>(pmt_t vector, size_t& len);
template const std::complex<float>* genVector_elements<std::complex<float>>(pmt_t vector, size_t& len);
template const std::complex<double>* genVector_elements<std::complex<double>>(pmt_t vector, size_t& len);



template <class T>
const T* genVector_raw(pmt_t vector)
{
  size_t len;
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_elements", vector);
  return _genVector<T>(vector)->elements(len);
}

template const uint8_t* genVector_raw<uint8_t>(pmt_t vector);
template const int8_t* genVector_raw<int8_t>(pmt_t vector);
template const uint16_t* genVector_raw<uint16_t>(pmt_t vector);
template const int16_t* genVector_raw<int16_t>(pmt_t vector);
template const uint32_t* genVector_raw<uint32_t>(pmt_t vector);
template const int32_t* genVector_raw<int32_t>(pmt_t vector);
template const uint64_t* genVector_raw<uint64_t>(pmt_t vector);
template const int64_t* genVector_raw<int64_t>(pmt_t vector);
template const float* genVector_raw<float>(pmt_t vector);
template const double* genVector_raw<double>(pmt_t vector);
template const std::complex<float>* genVector_raw<std::complex<float>>(pmt_t vector);
template const std::complex<double>* genVector_raw<std::complex<double>>(pmt_t vector);



template <class T>
const std::vector<T> genVector_elements(pmt_t vector)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_genVector_elements", vector);
  size_t len;
  const T* array = _genVector<T>(vector)->elements(len);
  const std::vector<T> vec(array, array + len);
  return vec;
}

template const std::vector<uint8_t> genVector_elements<uint8_t>(pmt_t vector);
template const std::vector<int8_t> genVector_elements<int8_t>(pmt_t vector);
template const std::vector<uint16_t> genVector_elements<uint16_t>(pmt_t vector);
template const std::vector<int16_t> genVector_elements<int16_t>(pmt_t vector);
template const std::vector<uint32_t> genVector_elements<uint32_t>(pmt_t vector);
template const std::vector<int32_t> genVector_elements<int32_t>(pmt_t vector);
template const std::vector<uint64_t> genVector_elements<uint64_t>(pmt_t vector);
template const std::vector<int64_t> genVector_elements<int64_t>(pmt_t vector);
template const std::vector<float> genVector_elements<float>(pmt_t vector);
template const std::vector<double> genVector_elements<double>(pmt_t vector);
template const std::vector<std::complex<float>> genVector_elements<std::complex<float>>(pmt_t vector);
template const std::vector<std::complex<double>> genVector_elements<std::complex<double>>(pmt_t vector);



template <class T>
T* genVector_writable_elements(pmt_t vector, size_t& len)
{
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_u8vector_writable_elements", vector);
  return _genVector<T>(vector)->writable_elements(len);
}

template uint8_t* genVector_writable_elements<uint8_t>(pmt_t vector, size_t& len);
template int8_t* genVector_writable_elements<int8_t>(pmt_t vector, size_t& len);
template uint16_t* genVector_writable_elements<uint16_t>(pmt_t vector, size_t& len);
template int16_t* genVector_writable_elements<int16_t>(pmt_t vector, size_t& len);
template uint32_t* genVector_writable_elements<uint32_t>(pmt_t vector, size_t& len);
template int32_t* genVector_writable_elements<int32_t>(pmt_t vector, size_t& len);
template uint64_t* genVector_writable_elements<uint64_t>(pmt_t vector, size_t& len);
template int64_t* genVector_writable_elements<int64_t>(pmt_t vector, size_t& len);
template float* genVector_writable_elements<float>(pmt_t vector, size_t& len);
template double* genVector_writable_elements<double>(pmt_t vector, size_t& len);
template std::complex<float>* genVector_writable_elements<std::complex<float>>(pmt_t vector, size_t& len);
template std::complex<double>* genVector_writable_elements<std::complex<double>>(pmt_t vector, size_t& len);



template <class T>
T* genVector_writable_raw(pmt_t vector)
{
  size_t len;
  if (!_genVector<T>(vector)->is_genVector())
    throw wrong_type("pmt_u8vector_writable_elements", vector);
  return _genVector<T>(vector)->writable_elements(len);
}

template uint8_t* genVector_writable_raw<uint8_t>(pmt_t vector);
template int8_t* genVector_writable_raw<int8_t>(pmt_t vector);
template uint16_t* genVector_writable_raw<uint16_t>(pmt_t vector);
template int16_t* genVector_writable_raw<int16_t>(pmt_t vector);
template uint32_t* genVector_writable_raw<uint32_t>(pmt_t vector);
template int32_t* genVector_writable_raw<int32_t>(pmt_t vector);
template uint64_t* genVector_writable_raw<uint64_t>(pmt_t vector);
template int64_t* genVector_writable_raw<int64_t>(pmt_t vector);
template float* genVector_writable_raw<float>(pmt_t vector);
template double* genVector_writable_raw<double>(pmt_t vector);
template std::complex<float>* genVector_writable_raw<std::complex<float>>(pmt_t vector);
template std::complex<double>* genVector_writable_raw<std::complex<double>>(pmt_t vector);

} /* namespace pmt */

} // namespace pl_proc
