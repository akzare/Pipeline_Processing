/**
 * @file   processor_factory.h
 *
 * @author Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *         Eric J. Mayo eric@pozicom.net
 *
 * @brief   processor_factory.h includes the factory design pattern for the all 
 *          processing classes in Pipeline Processing Framework.
 */

#ifndef PROCESSOR_FACTORY_H
#define PROCESSOR_FACTORY_H

#include "processor.h"

#include "vec_src_blk.h"
#include "adder_blk.h"
#include "vec_sink_blk.h"

#include <cstdint>
#include <memory>
#include <complex>
#include <vector>
#include <list>
#include <mutex>


namespace pl_proc {

/*!
 * \brief processor factory class for creation of different child classes of processor nodes.
 *  This class uses the concept of C++ template based on run-time string:
 *  See: https://stackoverflow.com/questions/38644146/choose-template-based-on-run-time-string-in-c
 */
class proc_factory{
public:

  /*!
   * \brief Adder processor node creator
   */
  template <typename... Args>
  static processor::sptr createADDER(const std::string& typeStr, Args&&... arg);

  /*!
   * \brief Source Generator processor node creator
   */
  template <typename... Args>
  static processor::sptr createSRC(const std::string& outTypeStr, Args&&... arg);

  /*!
   * \brief Sink processor node creator
   */
  template <typename... Args>
  static processor::sptr createSINK(const std::string& inTypeStr, Args&&... arg);
};


template <typename... Args>
typename processor::sptr proc_factory::createADDER(const std::string& typeStr, Args&&... arg)
{
  pmt::DataType type = pmt::TypeFromString(typeStr);
  const std::map<pmt::DataType, std::function<std::shared_ptr<processor>(Args&&...)>> factory{
    {pmt::DataType::UINT8,         [=](Args&&... args) { return std::make_shared<adder_blk<std::uint8_t>>(args...); } },
    {pmt::DataType::INT8,          [=](Args&&... args) { return std::make_shared<adder_blk<std::int8_t>>(args...); } },
    {pmt::DataType::UINT16,        [=](Args&&... args) { return std::make_shared<adder_blk<std::uint16_t>>(args...); } },
    {pmt::DataType::INT16,         [=](Args&&... args) { return std::make_shared<adder_blk<std::int16_t>>(args...); } },
    {pmt::DataType::UINT32,        [=](Args&&... args) { return std::make_shared<adder_blk<std::uint32_t>>(args...); } },
    {pmt::DataType::INT32,         [=](Args&&... args) { return std::make_shared<adder_blk<std::int32_t>>(args...); } },
    {pmt::DataType::UINT64,        [=](Args&&... args) { return std::make_shared<adder_blk<std::uint64_t>>(args...); } },
    {pmt::DataType::INT64,         [=](Args&&... args) { return std::make_shared<adder_blk<std::int64_t>>(args...); } },
    {pmt::DataType::FLOAT,         [=](Args&&... args) { return std::make_shared<adder_blk<float>>(args...); } },
    {pmt::DataType::COMPLEX_FLOAT, [=](Args&&... args) { return std::make_shared<adder_blk<std::complex<float>>>(args...); } },
    {pmt::DataType::UNKNOWN,       [=](Args&&... args) { return nullptr; } }
  };
  return factory.at(type)(std::forward<Args>(arg)...);
}

template <typename... Args>
typename processor::sptr proc_factory::createSRC(const std::string& outTypeStr, Args&&... arg)
{
  pmt::DataType outType = pmt::TypeFromString(outTypeStr);
  const std::map<pmt::DataType, std::function<std::shared_ptr<processor>(Args&&...)>> factory{
    {pmt::DataType::UINT8,          [=](Args&&... args) { return std::make_shared<vec_src_blk<std::uint8_t>>(args...); } },
    {pmt::DataType::INT8,           [=](Args&&... args) { return std::make_shared<vec_src_blk<std::int8_t>>(args...); } },
    {pmt::DataType::UINT16,         [=](Args&&... args) { return std::make_shared<vec_src_blk<std::uint16_t>>(args...); } },
    {pmt::DataType::INT16,          [=](Args&&... args) { return std::make_shared<vec_src_blk<std::int16_t>>(args...); } },
    {pmt::DataType::UINT32,         [=](Args&&... args) { return std::make_shared<vec_src_blk<std::uint32_t>>(args...); } },
    {pmt::DataType::INT32,          [=](Args&&... args) { return std::make_shared<vec_src_blk<std::int32_t>>(args...); } },
    {pmt::DataType::UINT64,         [=](Args&&... args) { return std::make_shared<vec_src_blk<std::uint64_t>>(args...); } },
    {pmt::DataType::INT64,          [=](Args&&... args) { return std::make_shared<vec_src_blk<std::int64_t>>(args...); } },
    {pmt::DataType::FLOAT,          [=](Args&&... args) { return std::make_shared<vec_src_blk<float>>(args...); } },
    {pmt::DataType::DOUBLE,         [=](Args&&... args) { return std::make_shared<vec_src_blk<double>>(args...); } },
    {pmt::DataType::COMPLEX_FLOAT,  [=](Args&&... args) { return std::make_shared<vec_src_blk<std::complex<float>>>(args...); } },
    {pmt::DataType::COMPLEX_DOUBLE, [=](Args&&... args) { return std::make_shared<vec_src_blk<std::complex<double>>>(args...); } },
    {pmt::DataType::UNKNOWN,        [=](Args&&... args) { return nullptr; } }
  };
  return factory.at(outType)(std::forward<Args>(arg)...);    
}

template <typename... Args>
typename processor::sptr proc_factory::createSINK(const std::string& inTypeStr, Args&&... arg)
{
  pmt::DataType inType = pmt::TypeFromString(inTypeStr);
  const std::map<pmt::DataType, std::function<std::shared_ptr<processor>(Args&&...)>> factory{
    {pmt::DataType::INT8,          [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::int8_t>>(args...); } },
    {pmt::DataType::UINT8,         [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::uint8_t>>(args...); } },
    {pmt::DataType::INT16,         [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::int16_t>>(args...); } },
    {pmt::DataType::UINT16,        [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::uint16_t>>(args...); } },
    {pmt::DataType::INT32,         [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::int32_t>>(args...); } },
    {pmt::DataType::UINT32,        [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::uint32_t>>(args...); } },
    {pmt::DataType::INT64,         [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::int64_t>>(args...); } },
    {pmt::DataType::UINT64,        [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::uint64_t>>(args...); } },
    {pmt::DataType::FLOAT,         [=](Args&&... args) { return std::make_shared<vec_sink_blk<float>>(args...); } },
    {pmt::DataType::COMPLEX_FLOAT, [=](Args&&... args) { return std::make_shared<vec_sink_blk<std::complex<float>>>(args...); } },
    {pmt::DataType::UNKNOWN,       [=](Args&&... args) { return nullptr; } }
  };

  return factory.at(inType)(std::forward<Args>(arg)...);
}

} // namespace pl_proc

#endif /* PROCESSOR_FACTORY_H */
