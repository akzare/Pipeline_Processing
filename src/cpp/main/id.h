/**
 * @file   id.h
 *
 * Copyright 2017 The Ray Authors. https://github.com/ray-project/ray
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * It has been modified & adapted to the Pipeline Processing Framework by:
 *   Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *   Eric J. Mayo eric@pozicom.net
 *
 * @brief   id.h includes the implementation for the ID generator class
 */

#ifndef PL_ID_H_
#define PL_ID_H_

#include "util.h"

#include <inttypes.h>
#include <limits.h>

#include <chrono>
#include <cstring>
#include <mutex>
#include <random>
#include <string>


namespace pl_proc {

class ObjectID;
class UniqueID;
class JobRunID;


/// The type of thes processor object.
enum class ModuleType : uint8_t {
  ADDER_MODULE                = 0x00,
  BER_BF_MODULE               = 0x01,
  CHUNKS2SYMB_MODULE          = 0x02,
  ENCODER_TRELLIS_MODULE      = 0x03,
  DECODER_LAZY_VITERBI_MODULE = 0x04,
  METRICS_MODULE              = 0x05,
  SRC_NOISE_MODULE            = 0x06,
  SRC_VEC_MODULE              = 0x07,
  SINK_VEC_MODULE             = 0x08,
  PACK_MODULE                 = 0x09,
  UNPACK_MODULE               = 0x0A,
};

using JobRunIDType = uint16_t;
using ObjectIDModuleType = uint8_t;
using ObjectIDModuleIndexType = uint8_t;
using ObjectIDPaketIndexType = uint32_t;


/// Length of FEC full-length IDs in bytes.
constexpr size_t kObjectIDSize = 8;


/// An ObjectID's bytes are split into the task ID itself and the index of the
/// object's creation. This is the maximum width of the object index in bits.
constexpr int kObjectIdModuleIndexSize = 8;
static_assert(kObjectIdModuleIndexSize % CHAR_BIT == 0,
              "ObjectIDModuleIndex prefix not a multiple of bytes");

constexpr int kObjectIdPaketIndexSize = 32;
static_assert(kObjectIdPaketIndexSize % CHAR_BIT == 0,
              "ObjectIDPaketIndex prefix not a multiple of bytes");

// Declaration.
uint64_t MurmurHash64A(const void *key, int len, unsigned int seed);

// Change the compiler alignment to 1 byte (default is 8).
#pragma pack(push, 1)


template <typename T>
class BaseID {
 public:
  BaseID();
  // Warning: this can duplicate IDs after a fork() call. We assume this never happens.
  static T FromRandom();
  static T FromBinary(const std::string &binary);
  static const T &Nil();
  static size_t Size() { return T::Size(); }

  size_t Hash() const;
  bool IsNil() const;
  bool operator==(const BaseID &rhs) const;
  bool operator!=(const BaseID &rhs) const;
  const uint8_t *Data() const;
  std::string Binary() const;
  std::string Hex() const;

 protected:
  BaseID(const std::string &binary) {
    CHECK(binary.size() == Size() || binary.size() == 0)
        << "expected size is " << Size() << ", but got " << binary.size();
    std::memcpy(const_cast<uint8_t *>(this->Data()), binary.data(), binary.size());
  }
  // All IDs are immutable for hash evaluations. MutableData is only allow to use
  // in construction time, so this function is protected.
  uint8_t *MutableData();
  // For lazy evaluation, be careful to have one Id contained in another.
  // This hash code will be duplicated.
  mutable size_t hash_ = 0;
};

class UniqueID : public BaseID<UniqueID> {
 public:
  static size_t Size() { return kObjectIDSize; }

  UniqueID() : BaseID() {}

 protected:
  UniqueID(const std::string &binary);

 protected:
  uint8_t id_[kObjectIDSize];
};

class JobRunID : public BaseID<JobRunID> {
 public:
  static constexpr int64_t kLength = 2;

  static JobRunID FromInt(uint16_t value);
  static JobRunID FromUniqueBytes(const JobRunID &job_id, const ObjectID &parent_task_id,
	        const size_t parent_task_counter);

  static size_t Size() { return kLength; }

  // Warning: this can duplicate IDs after a fork() call. We assume this never happens.
//  static JobRunID FromRandom() = delete;

  JobRunID() : BaseID() {}

  // conversion operator: return float value of fraction
  operator std::stringstream() const {
    std::stringstream os;
    for (int i: id_)  os << i;
    return os;
  }

  static JobRunID* getInstance()
  {
    if (instance_ == nullptr) {
      static JobRunID t = FromRandom();
      instance_ = &t;
    }

    return instance_;
  }

 private:
  uint8_t id_[kLength];
  static JobRunID *instance_;
};


class ObjectID : public BaseID<ObjectID> {
 private:
  static constexpr size_t kModuleTypeBytesLength = sizeof(ObjectIDModuleType);
  static constexpr size_t kModuleIndexBytesLength = sizeof(ObjectIDModuleIndexType);
  static constexpr size_t kPaketIndexBytesLength = sizeof(ObjectIDPaketIndexType);

 public:
  /// The maximum number of modules that can be returned or put by a task.
  static constexpr int64_t kMaxModuleIndex = ((int64_t)1 << kObjectIdModuleIndexSize) - 1;

  /// The maximum number of pakets that can be returned or put by a task.
  static constexpr int64_t kMaxPaketIndex = ((int64_t)1 << kObjectIdPaketIndexSize) - 1;

  /// The length of ObjectID in bytes.
  static constexpr size_t kLength =
      kModuleIndexBytesLength + kPaketIndexBytesLength + kModuleTypeBytesLength + JobRunID::kLength;

  ObjectID() : BaseID() {}

  /// The maximum index of object.
  ///
  /// It also means the max number of objects created (put or return) by one task.
  ///
  /// \return The maximum index of object.
  static uint64_t MaxModuleIndex() { return kMaxModuleIndex; }
  static uint64_t MaxPaketIndex() { return kMaxPaketIndex; }

  static size_t Size() { return kLength; }

  /// Get the index of this object in the task that created it.
  ///
  /// \return The index of object creation according to the task that created
  /// this object.
  ObjectIDModuleIndexType GetModuleIndex() const;
  ObjectIDPaketIndexType GetPaketIndex() const;
  JobRunIDType GetRunID() const;

  /// Compute the task ID of the job that created the object.
  ///
  /// \return The task ID of the task that created this object.
  JobRunID JobRunId() const;

  /// Return if this is a direct actor call object.
  ///
  /// \return True if this is a direct actor object return.
  bool IsUnpackModuleType() const {
    return GetModuleType() == static_cast<uint8_t>(ModuleType::UNPACK_MODULE);
  }

  /// Get the moduleType out of this object id.
  ObjectIDModuleType GetModuleType() const;

  /// Compute the object ID of an object put by the task.
  ///
  /// \param task_id The task ID of the task that created the object.
  /// \param index What index of the object put in the task.
  /// \param transport_type Which type of the transport that is used to
  ///        transfer this object.
  ///
  /// \return The computed object ID.
  static ObjectID ForModuleIndex(const JobRunID &run_id,
                                    ObjectIDModuleType module_type,
                                    ObjectIDModuleIndexType module_index,
                                    ObjectIDPaketIndexType paket_index);


 private:
  /// A helper method to generate an ObjectID.
  static ObjectID GenerateObjectId(const std::string &run_id_binary,
                                      ObjectIDModuleType module_type,
                                      ObjectIDModuleIndexType module_index,
                                      ObjectIDPaketIndexType paket_index);


 private:
  uint8_t id_[kLength];
};

//static_assert(sizeof(JobRunID) == JobRunID::kLength + sizeof(size_t) + sizeof(JobRunID*),
//              "JobRunID size is not as expected");
static_assert(sizeof(ObjectID) == ObjectID::kLength + sizeof(size_t),
              "ObjectID size is not as expected");

std::ostream &operator<<(std::ostream &os, const UniqueID &id);
std::ostream &operator<<(std::ostream &os, const JobRunID &id);
std::ostream &operator<<(std::ostream &os, const ObjectID &id);

#define DEFINE_UNIQUE_ID(type)                                                 \
  class type : public UniqueID {                                    \
   public:                                                                     \
    explicit type(const UniqueID &from) {                                      \
      std::memcpy(&id_, from.Data(), kObjectIDSize);                           \
    }                                                                          \
    type() : UniqueID() {}                                                     \
    static type FromRandom() { return type(UniqueID::FromRandom()); }          \
    static type FromBinary(const std::string &binary) { return type(binary); } \
    static type Nil() { return type(UniqueID::Nil()); }                        \
    static size_t Size() { return kObjectIDSize; }                             \
                                                                               \
   private:                                                                    \
    explicit type(const std::string &binary) {                                 \
      FEC_CHECK(binary.size() == Size() || binary.size() == 0)                 \
          << "expected size is " << Size() << ", but got " << binary.size();   \
      std::memcpy(&id_, binary.data(), binary.size());                         \
    }                                                                          \
  };

//DEFINE_UNIQUE_ID(FunctionID)

#undef DEFINE_UNIQUE_ID

// Restore the compiler alignment to default (8 bytes).
#pragma pack(pop)

template <typename T>
BaseID<T>::BaseID() {
  // Using const_cast to directly change data is dangerous. The cached
  // hash may not be changed. This is used in construction time.
  std::fill_n(this->MutableData(), T::Size(), 0xff);
}

template <typename T>
T BaseID<T>::FromRandom() {
  std::string data(T::Size(), 0);
  FillRandom(&data);
  return T::FromBinary(data);
}

template <typename T>
T BaseID<T>::FromBinary(const std::string &binary) {
  CHECK(binary.size() == T::Size() || binary.size() == 0)
      << "expected size is " << T::Size() << ", but got " << binary.size();
  T t = T::Nil();
  std::memcpy(t.MutableData(), binary.data(), binary.size());
  return t;
}

template <typename T>
const T &BaseID<T>::Nil() {
  static const T nil_id;
  return nil_id;
}

template <typename T>
bool BaseID<T>::IsNil() const {
  static T nil_id = T::Nil();
  return *this == nil_id;
}

template <typename T>
size_t BaseID<T>::Hash() const {
  // Note(ashione): hash code lazy calculation(it's invoked every time if hash code is
  // default value 0)
  if (!hash_) {
    hash_ = MurmurHash64A(Data(), T::Size(), 0);
  }
  return hash_;
}

template <typename T>
bool BaseID<T>::operator==(const BaseID &rhs) const {
  return std::memcmp(Data(), rhs.Data(), T::Size()) == 0;
}

template <typename T>
bool BaseID<T>::operator!=(const BaseID &rhs) const {
  return !(*this == rhs);
}

template <typename T>
uint8_t *BaseID<T>::MutableData() {
  return reinterpret_cast<uint8_t *>(this) + sizeof(hash_);
}

template <typename T>
const uint8_t *BaseID<T>::Data() const {
  return reinterpret_cast<const uint8_t *>(this) + sizeof(hash_);
}

template <typename T>
std::string BaseID<T>::Binary() const {
  return std::string(reinterpret_cast<const char *>(Data()), T::Size());
}

template <typename T>
std::string BaseID<T>::Hex() const {
  constexpr char hex[] = "0123456789abcdef";
  const uint8_t *id = Data();
  std::string result;
  for (int i = 0; i < T::Size(); i++) {
    unsigned int val = id[i];
    result.push_back(hex[val >> 4]);
    result.push_back(hex[val & 0xf]);
  }
  return result;
}

}  // namespace pl_proc

#endif  // PL_ID_H_
