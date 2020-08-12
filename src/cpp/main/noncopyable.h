/**
 * @file   noncopyable.h
 * @brief   noncopyable.h includes the implementation for a typical non-copyable class
 */

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H


namespace pl_proc {

//! @brief Inherit this to create a noncopyable object
class noncopyable
{
public:
  // Switched to C++11 way of doing it
  noncopyable(const noncopyable&) = delete;

  // Switched to C++11 way of doing it
  noncopyable& operator=(const noncopyable&) = delete;

protected:
  noncopyable() = default;
};

} // namespace pl_proc

#endif // NONCOPYABLE_H
