/**
 * @file   het_container.h
 *
 * This file is from: https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/
 * It has been adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   het_container.h includes the implementation of a heterogeneous 
 */


#ifndef H_CONTAINER_H
#define H_CONTAINER_H


#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <type_traits>

#include <cstdint>
#include <memory>
#include <vector>
#include <complex>
#include <list>

namespace pl_proc {

template<class...>
struct het_container_type_list{};

/*!
 * \brief Templated base class for visitors that defines a type_list.
 */
template<class... TYPES>
struct het_container_visitor_base
{
  using types = het_container_type_list<TYPES...>;
};


/*!
 * \brief Heterogeneous Container class is used to keep all processor nodes
 *        and can hold more than one data type.
 *  This class uses Variable templates feature that was added in C++14.
 *  Variable templates allow us to interpret a variable differently depending on a type.
 *  Also, this class uses Visitor design pattern to visit the container so that useful
 *  computation can be performed.
 *    
 *  For further information see: 
 *   Container and visitor design pattern for the all processing classes
 *    https://en.wikipedia.org/wiki/Visitor_pattern
 *    https://github.com/AnthonyCalandra/modern-cpp-features#variable-templates
 */
struct heterogeneous_container
{
public:
  heterogeneous_container() = default;
  heterogeneous_container(const heterogeneous_container& _other)
  {
    *this = _other;
  }

  heterogeneous_container& operator=(const heterogeneous_container& _other)
  {
    clear();
    clear_functions = _other.clear_functions;
    copy_functions = _other.copy_functions;
    size_functions = _other.size_functions;
    for (auto&& copy_function : copy_functions)
    {
      copy_function(_other, *this);
    }
    return *this;
  }

  template<class T>
  void push_back(const T&& _t)
  {
    // don't have it yet, so create functions for printing, copying, moving, and destroying
    if (items<T>.find(this) == std::end(items<T>))
    {
      clear_functions.emplace_back([](heterogeneous_container& _c){items<T>.erase(&_c);});
      // if someone copies me, they need to call each copy_function and pass themselves
      copy_functions.emplace_back([](const heterogeneous_container& _from, heterogeneous_container& _to)
      {
        items<T>[&_to] = items<T>[&_from];
      });
      size_functions.emplace_back([](const heterogeneous_container& _c){return items<T>[&_c].size();});
    }
    items<T>[this].push_back(_t);
  }

  void clear()
  {
    for (auto&& clear_func : clear_functions)
    {
      clear_func(*this);
    }
  }

  template<class T>
  size_t number_of() const
  {
    auto iter = items<T>.find(this);
    if (iter != items<T>.cend())
      return items<T>[this].size();
    return 0;
  }

  size_t size() const
  {
    size_t sum = 0;
    for (auto&& size_func : size_functions)
    {
      sum += size_func(*this);
    }
    // gotta be careful about this overflowing
    return sum;
  }

  ~heterogeneous_container()
  {
    clear();
  }

  template<class T>
  void visit_element(T&& visitor)
  {
    visit_element_impl(visitor, typename std::decay<T>::type::types{});
  }

  template<class T>
  void visit_elements(T&& visitor)
  {
    visit_elements_impl(visitor, typename std::decay<T>::type::types{});
  }

private:
  template<class T>
  static std::unordered_map<const heterogeneous_container*, std::vector<T>> items;

  template<class T, class U>
  using visit_function = decltype(std::declval<T>().operator()(std::declval<U&>()));
//  template<class T, class U>
//  static constexpr bool has_visit_v = std::experimental::is_detected<visit_function, T, U>::value;

  template<class T, template<class...> class TLIST, class... TYPES>
  void visit_element_impl(T&& visitor, TLIST<TYPES...>)
  {
    (..., visit_element_impl_help<typename std::decay<T>::type, TYPES>(visitor));
  }

  template<class T, class U>
  void visit_element_impl_help(T& visitor)
  {
//    static_assert(has_visit_v<T, U>, "Visitors must provide a visit_element function accepting a reference for each type");
    for (auto&& element : items<U>[this])
    {
      visitor(element);
    }
  }

  template<class T, template<class...> class TLIST, class... TYPES>
  void visit_elements_impl(T&& visitor, TLIST<TYPES...>)
  {
    (..., visit_elements_impl_help<typename std::decay<T>::type, TYPES>(visitor));
  }

  template<class T, class U>
  void visit_elements_impl_help(T& visitor)
  {
//    static_assert(has_visit_v<T, U>, "Visitors must provide a visit_element function accepting a reference for each type");
    visitor(items<U>[this]);
  }

  std::vector<std::function<void(heterogeneous_container&)>> clear_functions;
  std::vector<std::function<void(const heterogeneous_container&, heterogeneous_container&)>> copy_functions;
  std::vector<std::function<size_t(const heterogeneous_container&)>> size_functions;
};

template<class T>
std::unordered_map<const heterogeneous_container*, std::vector<T>> heterogeneous_container::items;

} // namespace pl_proc

#endif /* H_CONTAINER_H */
