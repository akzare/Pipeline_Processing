/* -*- c++ -*- */
/*
 * Copyright 2006,2009,2010,2013 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  It has been modified & adapted to the Pipeline Processing Framework with
 *  adding DataType & genVector by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *
 *
 */

#ifndef INCLUDED_PMT_H
#define INCLUDED_PMT_H

#include "noncopyable.h"

#include <stdint.h>
#include <complex>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>


namespace pl_proc {

namespace pmt {
/*!
 * This file defines a polymorphic type and the operations on it.
 *
 * It draws heavily on the idea of scheme and lisp data types.
 * The interface parallels that in Guile 1.8, with the notable
 * exception that these objects are transparently reference counted.
 */


enum class DataType : uint8_t {
  UNKNOWN             = 0x00,

  GVEC_UINT8          = 0x01,
  GVEC_INT8           = 0x02,
  GVEC_UINT16         = 0x03,
  GVEC_INT16          = 0x04,
  GVEC_INT32          = 0x05,
  GVEC_UINT32         = 0x06,
  GVEC_INT64          = 0x07,
  GVEC_UINT64         = 0x08,
  GVEC_FLOAT          = 0x09,
  GVEC_DOUBLE         = 0x0A,
  GVEC_COMPLEX_FLOAT  = 0x0B,
  GVEC_COMPLEX_DOUBLE = 0x0C,

  UINT8               = 0x21,
  INT8                = 0x22,
  UINT16              = 0x23,
  INT16               = 0x24,
  INT32               = 0x25,
  UINT32              = 0x26,
  INT64               = 0x27,
  UINT64              = 0x28,
  FLOAT               = 0x29,
  DOUBLE              = 0x2A,
  COMPLEX_FLOAT       = 0x2B,
  COMPLEX_DOUBLE      = 0x2C,

  PAIR                = 0x31,
  VECTOR              = 0x32,
  DICT                = 0x33,
  TUPLE               = 0x34,
};



inline DataType TypeFromString(const std::string& s)
{
  if (s == "GVEC_UINT8")           { return DataType::GVEC_UINT8; }
  if (s == "GVEC_INT8")            { return DataType::GVEC_INT8; }
  if (s == "GVEC_UINT16")          { return DataType::GVEC_UINT16; }
  if (s == "GVEC_INT16")           { return DataType::GVEC_INT16; }
  if (s == "GVEC_INT32")           { return DataType::GVEC_INT32; }
  if (s == "GVEC_UINT32")          { return DataType::GVEC_UINT32; }
  if (s == "GVEC_INT64")           { return DataType::GVEC_INT64; }
  if (s == "GVEC_UINT64")          { return DataType::GVEC_UINT64; }
  if (s == "GVEC_FLOAT")           { return DataType::GVEC_FLOAT; }
  if (s == "GVEC_DOUBLE")          { return DataType::GVEC_DOUBLE; }
  if (s == "GVEC_COMPLEX_FLOAT")   { return DataType::GVEC_COMPLEX_FLOAT; }
  if (s == "GVEC_COMPLEX_DOUBLE")  { return DataType::GVEC_COMPLEX_DOUBLE; }
  if (s == "UINT8")                { return DataType::UINT8; }
  if (s == "INT8")                 { return DataType::INT8; }
  if (s == "UINT16")               { return DataType::UINT16; }
  if (s == "INT16")                { return DataType::INT16; }
  if (s == "INT32")                { return DataType::INT32; }
  if (s == "UINT32")               { return DataType::UINT32; }
  if (s == "INT64")                { return DataType::INT64; }
  if (s == "UINT64")               { return DataType::UINT64; }
  if (s == "FLOAT")                { return DataType::FLOAT; }
  if (s == "DOUBLE")               { return DataType::DOUBLE; }
  if (s == "COMPLEX_FLOAT")        { return DataType::COMPLEX_FLOAT; }
  if (s == "COMPLEX_DOUBLE")       { return DataType::COMPLEX_DOUBLE; }
  if (s == "PAIR")                 { return DataType::PAIR; }
  if (s == "VECTOR")               { return DataType::VECTOR; }
  if (s == "DICT")                 { return DataType::DICT; }
  if (s == "TUPLE")                { return DataType::TUPLE; }

  return DataType::UNKNOWN;
}


/*!
 * \brief base class of all pmt types
 */
class pmt_base : noncopyable
{

public:
  pmt_base(){};
  virtual ~pmt_base();

  virtual bool is_bool() const { return false; }
  virtual bool is_symbol() const { return false; }
  virtual bool is_number() const { return false; }
  virtual bool is_integer() const { return false; }
  virtual bool is_uint64() const { return false; }
  virtual bool is_real() const { return false; }
  virtual bool is_complex() const { return false; }
  virtual bool is_null() const { return false; }
  virtual bool is_pair() const { return false; }
  virtual bool is_tuple() const { return false; }
  virtual bool is_vector() const { return false; }
  virtual bool is_dict() const { return false; }
  virtual bool is_any() const { return false; }

  virtual bool is_genVector() const { return false; }
  DataType getType_genVector() const { return DataType::UNKNOWN; }

  virtual bool is_uniform_vector() const { return false; }
  virtual size_t getLength_genVector() const { return 0; }
  virtual size_t getItemSize_genVector() const { return 0; }
};

/*!
 * \brief typedef for shared pointer (transparent reference counting).
 * See http://www.boost.org/libs/smart_ptr/smart_ptr.htm
 */
typedef std::shared_ptr<pmt_base> pmt_t;

class exception : public std::logic_error
{
public:
  exception(const std::string& msg, pmt_t obj);
};

class wrong_type : public std::invalid_argument
{
public:
  wrong_type(const std::string& msg, pmt_t obj);
};

class out_of_range : public exception
{
public:
  out_of_range(const std::string& msg, pmt_t obj);
};

class notimplemented : public exception
{
public:
  notimplemented(const std::string& msg, pmt_t obj);
};


/*
 * ------------------------------------------------------------------------
 * Constants
 * ------------------------------------------------------------------------
 */

pmt_t get_PMT_NIL();
pmt_t get_PMT_T();
pmt_t get_PMT_F();
pmt_t get_PMT_EOF();

#define PMT_NIL get_PMT_NIL()
#define PMT_T get_PMT_T()
#define PMT_F get_PMT_F()
#define PMT_EOF get_PMT_EOF()


/*
 * ------------------------------------------------------------------------
 * Booleans.  Two constants, #t and #f.
 *
 * In predicates, anything that is not #f is considered true.
 * I.e., there is a single false value, #f.
 * ------------------------------------------------------------------------
 */

//! Return true if obj is \#t or \#f, else return false.
bool is_bool(pmt_t obj);

//! Return false if obj is \#f, else return true.
bool is_true(pmt_t obj);

//! Return true if obj is \#f, else return true.
bool is_false(pmt_t obj);

//! Return \#f is val is false, else return \#t.
pmt_t from_bool(bool val);

//! Return true if val is pmt::True, return false when val is pmt::PMT_F,
// else raise wrong_type exception.
bool to_bool(pmt_t val);

/*
 * ------------------------------------------------------------------------
 *			       Symbols
 * ------------------------------------------------------------------------
 */

//! Return true if obj is a symbol, else false.
bool is_symbol(const pmt_t& obj);

//! Return the symbol whose name is \p s.
pmt_t string_to_symbol(const std::string& s);

//! Alias for pmt_string_to_symbol
pmt_t intern(const std::string& s);


/*!
 * If \p is a symbol, return the name of the symbol as a string.
 * Otherwise, raise the wrong_type exception.
 */
const std::string symbol_to_string(const pmt_t& sym);

/*
 * ------------------------------------------------------------------------
 *           Numbers: we support integer, real and complex
 * ------------------------------------------------------------------------
 */

//! Return true if obj is any kind of number, else false.
bool is_number(pmt_t obj);

/*
 * ------------------------------------------------------------------------
 *			       Integers
 * ------------------------------------------------------------------------
 */

//! Return true if \p x is an integer number, else false
bool is_integer(pmt_t x);

//! Return the pmt value that represents the integer \p x.
pmt_t from_long(long x);

/*!
 * \brief Convert pmt to long if possible.
 *
 * When \p x represents an exact integer that fits in a long,
 * return that integer.  Else raise an exception, either wrong_type
 * when x is not an exact integer, or out_of_range when it doesn't fit.
 */
long to_long(pmt_t x);

/*
 * ------------------------------------------------------------------------
 *			       uint64_t
 * ------------------------------------------------------------------------
 */

//! Return true if \p x is an uint64 number, else false
bool is_uint64(pmt_t x);

//! Return the pmt value that represents the uint64 \p x.
pmt_t from_uint64(uint64_t x);

/*!
 * \brief Convert pmt to uint64 if possible.
 *
 * When \p x represents an exact integer that fits in a uint64,
 * return that uint64.  Else raise an exception, either wrong_type
 * when x is not an exact uint64, or out_of_range when it doesn't fit.
 */
uint64_t to_uint64(pmt_t x);

/*
 * ------------------------------------------------------------------------
 *				Reals
 * ------------------------------------------------------------------------
 */

/*
 * \brief Return true if \p obj is a real number, else false.
 */
bool is_real(pmt_t obj);

//! Return the pmt value that represents double \p x.
pmt_t from_double(double x);
pmt_t from_float(float x);

/*!
 * \brief Convert pmt to double if possible.
 *
 * Returns the number closest to \p val that is representable
 * as a double.  The argument \p val must be a real or integer, otherwise
 * a wrong_type exception is raised.
 */
double to_double(pmt_t x);

/*!
 * \brief Convert pmt to float if possible.
 *
 * This basically is to_double() with a type-cast; the PMT stores
 * the value as a double in any case. Use this when strict typing
 * is required.
 */
float to_float(pmt_t x);

/*
 * ------------------------------------------------------------------------
 *			       Complex
 * ------------------------------------------------------------------------
 */

/*!
 * \brief return true if \p obj is a complex number, false otherwise.
 */
bool is_complex(pmt_t obj);

//! Return a complex number constructed of the given real and imaginary parts.
pmt_t make_rectangular(double re, double im);

//! Return a complex number constructed of the given real and imaginary parts.
pmt_t from_complex(double re, double im);

//! Return a complex number constructed of the given a complex number.
pmt_t from_complex(const std::complex<double>& z);

//! Return a complex number constructed of the given real and imaginary parts.
pmt_t pmt_from_complex(double re, double im);

//! Return a complex number constructed of the given a complex number.
pmt_t pmt_from_complex(const std::complex<double>& z);

/*!
 * If \p z is complex, real or integer, return the closest complex<double>.
 * Otherwise, raise the wrong_type exception.
 */
std::complex<double> to_complex(pmt_t z);

/*
 * ------------------------------------------------------------------------
 *				Pairs
 * ------------------------------------------------------------------------
 */

//! Return true if \p x is the empty list, otherwise return false.
bool is_null(const pmt_t& x);

//! Return true if \p obj is a pair, else false (warning: also returns true for a dict)
bool is_pair(const pmt_t& obj);

//! Return a newly allocated pair whose car is \p x and whose cdr is \p y.
pmt_t cons(const pmt_t& x, const pmt_t& y);

//! If \p pair is a pair, return the car of the \p pair, otherwise raise wrong_type.
pmt_t car(const pmt_t& pair);

//! If \p pair is a pair, return the cdr of the \p pair, otherwise raise wrong_type.
pmt_t cdr(const pmt_t& pair);

//! Stores \p value in the car field of \p pair.
void set_car(pmt_t pair, pmt_t value);

//! Stores \p value in the cdr field of \p pair.
void set_cdr(pmt_t pair, pmt_t value);

pmt_t caar(pmt_t pair);
pmt_t cadr(pmt_t pair);
pmt_t cdar(pmt_t pair);
pmt_t cddr(pmt_t pair);
pmt_t caddr(pmt_t pair);
pmt_t cadddr(pmt_t pair);

/*
 * ------------------------------------------------------------------------
 *			          Tuples
 *
 * Store a fixed number of objects.  Tuples are not modifiable, and thus
 * are excellent for use as messages.  Indexing is zero based.
 * Access time to an element is O(1).
 * ------------------------------------------------------------------------
 */

//! Return true if \p x is a tuple, otherwise false.
bool is_tuple(pmt_t x);

pmt_t make_tuple();
pmt_t make_tuple(const pmt_t& e0);
pmt_t make_tuple(const pmt_t& e0, const pmt_t& e1);
pmt_t make_tuple(const pmt_t& e0, const pmt_t& e1, const pmt_t& e2);
pmt_t make_tuple(const pmt_t& e0,
                 const pmt_t& e1,
                 const pmt_t& e2,
                 const pmt_t& e3);
pmt_t make_tuple(
        const pmt_t& e0, const pmt_t& e1, const pmt_t& e2, const pmt_t& e3, const pmt_t& e4);
pmt_t make_tuple(const pmt_t& e0,
                 const pmt_t& e1,
                 const pmt_t& e2,
                 const pmt_t& e3,
                 const pmt_t& e4,
                 const pmt_t& e5);
pmt_t make_tuple(const pmt_t& e0,
                 const pmt_t& e1,
                 const pmt_t& e2,
                 const pmt_t& e3,
                 const pmt_t& e4,
                 const pmt_t& e5,
                 const pmt_t& e6);
pmt_t make_tuple(const pmt_t& e0,
                 const pmt_t& e1,
                 const pmt_t& e2,
                 const pmt_t& e3,
                 const pmt_t& e4,
                 const pmt_t& e5,
                 const pmt_t& e6,
                 const pmt_t& e7);
pmt_t make_tuple(const pmt_t& e0,
                 const pmt_t& e1,
                 const pmt_t& e2,
                 const pmt_t& e3,
                 const pmt_t& e4,
                 const pmt_t& e5,
                 const pmt_t& e6,
                 const pmt_t& e7,
                 const pmt_t& e8);
pmt_t make_tuple(const pmt_t& e0,
                 const pmt_t& e1,
                 const pmt_t& e2,
                 const pmt_t& e3,
                 const pmt_t& e4,
                 const pmt_t& e5,
                 const pmt_t& e6,
                 const pmt_t& e7,
                 const pmt_t& e8,
                 const pmt_t& e9);

/*!
 * If \p x is a vector or proper list, return a tuple containing the elements of x
 */
pmt_t to_tuple(const pmt_t& x);

/*!
 * Return the contents of position \p k of \p tuple.
 * \p k must be a valid index of \p tuple.
 */
pmt_t tuple_ref(const pmt_t& tuple, size_t k);

/*
 * ------------------------------------------------------------------------
 *			       Vectors
 *
 * These vectors can hold any kind of objects.  Indexing is zero based.
 * ------------------------------------------------------------------------
 */

//! Return true if \p x is a vector, otherwise false.
bool is_vector(pmt_t x);

//! Make a vector of length \p k, with initial values set to \p fill
pmt_t make_vector(size_t k, pmt_t fill);

/*!
 * Return the contents of position \p k of \p vector.
 * \p k must be a valid index of \p vector.
 */
pmt_t vector_ref(pmt_t vector, size_t k);

//! Store \p obj in position \p k.
void vector_set(pmt_t vector, size_t k, pmt_t obj);

//! Store \p fill in every position of \p vector
void vector_fill(pmt_t vector, pmt_t fill);

/*
 * ------------------------------------------------------------------------
 *		      Binary Large Objects (BLOBs)
 *
 * Handy for passing around uninterpreted chunks of memory.
 * ------------------------------------------------------------------------
 */

//! Return true if \p x is a blob, otherwise false.
bool is_blob(pmt_t x);

/*!
 * \brief Make a blob given a pointer and length in bytes
 *
 * \param buf is the pointer to data to use to create blob
 * \param len is the size of the data in bytes.
 *
 * The data is copied into the blob.
 */
pmt_t make_blob(const void* buf, size_t len);

//! Return a pointer to the blob's data
const void* blob_data(pmt_t blob);

//! Return the blob's length in bytes
size_t blob_length(pmt_t blob);

/*!
 * <pre>
 *		       Uniform Numeric Vectors
 *
 * A uniform numeric vector is a vector whose elements are all of single
 * numeric type.  pmt offers uniform numeric vectors for signed and
 * unsigned 8-bit, 16-bit, 32-bit, and 64-bit integers, two sizes of
 * floating point values, and complex floating-point numbers of these
 * two sizes.  Indexing is zero based.
 *
 * The names of the functions include these tags in their names:
 *
 *    u8  unsigned 8-bit integers
 *    s8  signed 8-bit integers
 *   u16  unsigned 16-bit integers
 *   s16  signed 16-bit integers
 *   u32  unsigned 32-bit integers
 *   s32  signed 32-bit integers
 *   u64  unsigned 64-bit integers
 *   s64  signed 64-bit integers
 *   f32  the C++ type float
 *   f64  the C++ type double
 *   c32  the C++ type complex<float>
 *   c64  the C++ type complex<double>
 * </pre>
 */

//! true if \p x is any kind of uniform numeric vector
bool is_uniform_vector(pmt_t x);
template <class T> DataType getType_genVector(pmt_t x);
template <class T> size_t getLength_genVector(pmt_t x);
template <class T> size_t getItemSize_genVector(pmt_t x);
template <class T> bool is_genVector(pmt_t x);

//! item size in bytes if \p x is any kind of uniform numeric vector
size_t uniform_vector_itemsize(pmt_t x);

template <class T> pmt_t make_genVector(size_t k, T fill);
template <class T> pmt_t make_genVector(size_t k);
template <class T> pmt_t init_genVector(size_t k, const T* data);
template <class T> pmt_t init_genVector(size_t k, const std::vector<T>& data);
template <class T> T genVector_ref(pmt_t v, size_t k);
template <class T> void genVector_set(pmt_t v, size_t k, T x);
template <class T> void genVector_fill(pmt_t v, T x);
// Return const pointers to the elements

const void*
uniform_vector_elements(pmt_t v, size_t& len); //< works with any; len is in bytes

template <class T>
const void* genVector_uniform_elements(pmt_t v, size_t& len);

template <class T>
void* genVector_uniform_writable_elements(pmt_t v, size_t& len);

template <class T> const T* genVector_elements(pmt_t v, size_t& len);   //< len is in elements
template <class T> const T* genVector_raw(pmt_t v);   //< len is in elements
// len is in elements
template <class T> const std::vector<T> genVector_elements(pmt_t v);
// len is in elements
template <class T> const std::vector<T> pmt_genVector_elements(pmt_t v);
// Return non-const pointers to the elements

void*
uniform_vector_writable_elements(pmt_t v,
                                 size_t& len); //< works with any; len is in bytes

template <class T> T* genVector_writable_elements(pmt_t v, size_t& len); //< len is in elements
template <class T> T* genVector_writable_raw(pmt_t v); //< len is in elements

/*
 * ------------------------------------------------------------------------
 *	   Dictionary (a.k.a associative array, hash, map)
 *
 * This is a functional data structure that is persistent.  Updating a
 * functional data structure does not destroy the existing version, but
 * rather creates a new version that coexists with the old.
 * ------------------------------------------------------------------------
 */

//! Return true if \p obj is a dictionary (warning: also returns true for a pair)
bool is_dict(const pmt_t& obj);

//! Make an empty dictionary
pmt_t make_dict();

//! Return a new dictionary with \p key associated with \p value.
pmt_t dict_add(const pmt_t& dict, const pmt_t& key, const pmt_t& value);

//! Return a new dictionary with \p key removed.
pmt_t dict_delete(const pmt_t& dict, const pmt_t& key);

//! Return true if \p key exists in \p dict
bool dict_has_key(const pmt_t& dict, const pmt_t& key);

//! If \p key exists in \p dict, return associated value; otherwise return \p not_found.
pmt_t dict_ref(const pmt_t& dict, const pmt_t& key, const pmt_t& not_found);

//! Return list of (key . value) pairs
pmt_t dict_items(pmt_t dict);

//! Return list of keys
pmt_t dict_keys(pmt_t dict);

//! Return a new dictionary \p dict1 with k=>v pairs from \p dict2 added.
pmt_t dict_update(const pmt_t& dict1, const pmt_t& dict2);

//! Return list of values
pmt_t dict_values(pmt_t dict);

/*
 * ------------------------------------------------------------------------
 *			  General functions
 * ------------------------------------------------------------------------
 */

//! Return true if x and y are the same object; otherwise return false.
bool eq(const pmt_t& x, const pmt_t& y);

/*!
 * \brief Return true if x and y should normally be regarded as the same object, else
 * false.
 *
 * <pre>
 * eqv returns true if:
 *   x and y are the same object.
 *   x and y are both \#t or both \#f.
 *   x and y are both symbols and their names are the same.
 *   x and y are both numbers, and are numerically equal.
 *   x and y are both the empty list (nil).
 *   x and y are pairs or vectors that denote same location in store.
 * </pre>
 */
bool eqv(const pmt_t& x, const pmt_t& y);

/*!
 * pmt::equal recursively compares the contents of pairs and vectors,
 * applying pmt::eqv on other objects such as numbers and symbols.
 * pmt::equal may fail to terminate if its arguments are circular data
 * structures.
 */
bool equal(const pmt_t& x, const pmt_t& y);


//! Return the number of elements in v
size_t length(const pmt_t& v);

/*!
 * \brief Find the first pair in \p alist whose car field is \p obj
 *  and return that pair.
 *
 * \p alist (for "association list") must be a list of pairs.  If no pair
 * in \p alist has \p obj as its car then \#f is returned.
 * Uses pmt::eq to compare \p obj with car fields of the pairs in \p alist.
 */
pmt_t assq(pmt_t obj, pmt_t alist);

/*!
 * \brief Find the first pair in \p alist whose car field is \p obj
 *  and return that pair.
 *
 * \p alist (for "association list") must be a list of pairs.  If no pair
 * in \p alist has \p obj as its car then \#f is returned.
 * Uses pmt::eqv to compare \p obj with car fields of the pairs in \p alist.
 */
pmt_t assv(pmt_t obj, pmt_t alist);

/*!
 * \brief Find the first pair in \p alist whose car field is \p obj
 *  and return that pair.
 *
 * \p alist (for "association list") must be a list of pairs.  If no pair
 * in \p alist has \p obj as its car then \#f is returned.
 * Uses pmt::equal to compare \p obj with car fields of the pairs in \p alist.
 */
pmt_t assoc(pmt_t obj, pmt_t alist);

/*!
 * \brief Apply \p proc element-wise to the elements of list and returns
 * a list of the results, in order.
 *
 * \p list must be a list.  The dynamic order in which \p proc is
 * applied to the elements of \p list is unspecified.
 */
pmt_t map(pmt_t proc(const pmt_t&), pmt_t list);

/*!
 * \brief reverse \p list.
 *
 * \p list must be a proper list.
 */
pmt_t reverse(pmt_t list);

/*!
 * \brief destructively reverse \p list.
 *
 * \p list must be a proper list.
 */
pmt_t reverse_x(pmt_t list);

/*!
 * \brief (acons x y a) == (cons (cons x y) a)
 */
inline static pmt_t acons(pmt_t x, pmt_t y, pmt_t a) { return cons(cons(x, y), a); }

/*!
 * \brief locates \p nth element of \n list where the car is the 'zeroth' element.
 */
pmt_t nth(size_t n, pmt_t list);

/*!
 * \brief returns the tail of \p list that would be obtained by calling
 * cdr \p n times in succession.
 */
pmt_t nthcdr(size_t n, pmt_t list);

/*!
 * \brief Return the first sublist of \p list whose car is \p obj.
 * If \p obj does not occur in \p list, then \#f is returned.
 * pmt::memq use pmt::eq to compare \p obj with the elements of \p list.
 */
pmt_t memq(pmt_t obj, pmt_t list);

/*!
 * \brief Return the first sublist of \p list whose car is \p obj.
 * If \p obj does not occur in \p list, then \#f is returned.
 * pmt::memv use pmt::eqv to compare \p obj with the elements of \p list.
 */
pmt_t memv(pmt_t obj, pmt_t list);

/*!
 * \brief Return the first sublist of \p list whose car is \p obj.
 * If \p obj does not occur in \p list, then \#f is returned.
 * pmt::member use pmt::equal to compare \p obj with the elements of \p list.
 */
pmt_t member(pmt_t obj, pmt_t list);

/*!
 * \brief Return true if every element of \p list1 appears in \p list2, and false
 * otherwise. Comparisons are done with pmt::eqv.
 */
bool subsetp(pmt_t list1, pmt_t list2);

/*!
 * \brief Return a list of length 1 containing \p x1
 */
pmt_t list1(const pmt_t& x1);

/*!
 * \brief Return a list of length 2 containing \p x1, \p x2
 */
pmt_t list2(const pmt_t& x1, const pmt_t& x2);

/*!
 * \brief Return a list of length 3 containing \p x1, \p x2, \p x3
 */
pmt_t list3(const pmt_t& x1, const pmt_t& x2, const pmt_t& x3);

/*!
 * \brief Return a list of length 4 containing \p x1, \p x2, \p x3, \p x4
 */
pmt_t list4(const pmt_t& x1, const pmt_t& x2, const pmt_t& x3, const pmt_t& x4);

/*!
 * \brief Return a list of length 5 containing \p x1, \p x2, \p x3, \p x4, \p x5
 */
pmt_t list5(
        const pmt_t& x1, const pmt_t& x2, const pmt_t& x3, const pmt_t& x4, const pmt_t& x5);

/*!
 * \brief Return a list of length 6 containing \p x1, \p x2, \p x3, \p x4, \p
 * x5, \p x6
 */
pmt_t list6(const pmt_t& x1,
                    const pmt_t& x2,
                    const pmt_t& x3,
                    const pmt_t& x4,
                    const pmt_t& x5,
                    const pmt_t& x6);

/*!
 * \brief Return \p list with \p item added to it.
 */
pmt_t list_add(pmt_t list, const pmt_t& item);

/*!
 * \brief Return \p list with \p item removed from it.
 */
pmt_t list_rm(pmt_t list, const pmt_t& item);

/*!
 * \brief Return bool of \p list contains \p item
 */
bool list_has(pmt_t list, const pmt_t& item);


/*
 * ------------------------------------------------------------------------
 *			     read / write
 * ------------------------------------------------------------------------
 */

//! return true if obj is the EOF object, otherwise return false.
bool is_eof_object(pmt_t obj);

/*!
 * read converts external representations of pmt objects into the
 * objects themselves.  Read returns the next object parsable from
 * the given input port, updating port to point to the first
 * character past the end of the external representation of the
 * object.
 *
 * If an end of file is encountered in the input before any
 * characters are found that can begin an object, then an end of file
 * object is returned.   The port remains open, and further attempts
 * to read will also return an end of file object.  If an end of file
 * is encountered after the beginning of an object's external
 * representation, but the external representation is incomplete and
 * therefore not parsable, an error is signaled.
 */
pmt_t read(std::istream& port);

/*!
 * Write a written representation of \p obj to the given \p port.
 */
void write(pmt_t obj, std::ostream& port);

/*!
 * Return a string representation of \p obj.
 * This is the same output as would be generated by pmt::write.
 */
std::string write_string(pmt_t obj);


std::ostream& operator<<(std::ostream& os, pmt_t obj);

/*!
 * \brief Write pmt string representation to stdout.
 */
void print(pmt_t v);


/*
 * ------------------------------------------------------------------------
 *		      portable byte stream representation
 * ------------------------------------------------------------------------
 */
/*!
 * \brief Write portable byte-serial representation of \p obj to \p sink
 */
bool serialize(pmt_t obj, std::streambuf& sink);

/*!
 * \brief Create obj from portable byte-serial representation
 */
pmt_t deserialize(std::streambuf& source);


void dump_sizeof(); // debugging

/*!
 * \brief Provide a simple string generating interface to pmt's serialize function
 */
std::string serialize_str(pmt_t obj);

/*!
 * \brief Provide a simple string generating interface to pmt's deserialize function
 */
pmt_t deserialize_str(std::string str);

/*!
 * \brief Provide a comparator function object to allow pmt use in stl types
 */
class comparator
{
public:
    bool operator()(pmt_t const& p1, pmt_t const& p2) const
    {
        return eqv(p1, p2) ? false : p1.get() > p2.get();
    }
};

} /* namespace pmt */

} // namespace pl_proc

#endif /* INCLUDED_PMT_H */
