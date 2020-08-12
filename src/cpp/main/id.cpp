/**
 * @file   id.cpp
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
 * @brief   id.cpp includes the implementation for the ID generator class
 */

#include "id.h"
#include "util.h"

extern "C" {
#include "sha256.h"
}

#include <limits.h>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <random>
#include <vector>



// Definitions for computing hash digests.
#define DIGEST_SIZE SHA256_BLOCK_SIZE

namespace pl_proc {

JobRunID *JobRunID::instance_ = nullptr;


uint64_t MurmurHash64A(const void *key, int len, unsigned int seed);

/// A helper function to generate the unique bytes by hash.
std::string GenerateUniqueBytes(const JobRunID &job_id, const ObjectID &parent_task_id,
                                size_t parent_task_counter, size_t length) {
  CHECK(length <= DIGEST_SIZE);
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, reinterpret_cast<const BYTE *>(job_id.Data()), job_id.Size());
  sha256_update(&ctx, reinterpret_cast<const BYTE *>(parent_task_id.Data()),
                parent_task_id.Size());
  sha256_update(&ctx, (const BYTE *)&parent_task_counter, sizeof(parent_task_counter));

  BYTE buff[DIGEST_SIZE];
  sha256_final(&ctx, buff);
  return std::string(buff, buff + length);
}

namespace {


}  // namespace

template <typename T>
void FillNil(T *data) {
  FEC_CHECK(data != nullptr);
  for (size_t i = 0; i < data->size(); i++) {
    (*data)[i] = static_cast<uint8_t>(0xFF);
  }
}

ObjectIDModuleType ObjectID::GetModuleType() const {
  ObjectIDModuleType moduleType;
  std::memcpy(&moduleType, id_ + JobRunID::kLength, sizeof(moduleType));
  return moduleType;
}


// This code is from https://sites.google.com/site/murmurhash/
// and is public domain.
uint64_t MurmurHash64A(const void *key, int len, unsigned int seed) {
  const uint64_t m = 0xc6a4a7935bd1e995;
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t *data = reinterpret_cast<const uint64_t *>(key);
  const uint64_t *end = data + (len / 8);

  while (data != end) {
    uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const unsigned char *data2 = reinterpret_cast<const unsigned char *>(data);

  switch (len & 7) {
  case 7:
    h ^= uint64_t(data2[6]) << 48;
  case 6:
    h ^= uint64_t(data2[5]) << 40;
  case 5:
    h ^= uint64_t(data2[4]) << 32;
  case 4:
    h ^= uint64_t(data2[3]) << 24;
  case 3:
    h ^= uint64_t(data2[2]) << 16;
  case 2:
    h ^= uint64_t(data2[1]) << 8;
  case 1:
    h ^= uint64_t(data2[0]);
    h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

JobRunID ObjectID::JobRunId() const {
  return JobRunID::FromBinary(
      std::string(reinterpret_cast<const char *>(id_), JobRunID::Size()));
}

ObjectID ObjectID::ForModuleIndex(const JobRunID &run_id,
                                  ObjectIDModuleType module_type,
                                  ObjectIDModuleIndexType module_index,
                                  ObjectIDPaketIndexType paket_index) {
  CHECK(module_index >= 0 && module_index < kMaxModuleIndex) << "module index=" << module_index;
  CHECK(paket_index >= 0 && paket_index < kMaxPaketIndex) << "paket index=" << paket_index;
  return GenerateObjectId(run_id.Binary(), module_type, module_index, paket_index);
}

JobRunIDType ObjectID::GetRunID() const {
  JobRunIDType id;
  std::memcpy(&id, id_ , JobRunID::kLength);
  return id;
}

ObjectIDModuleIndexType ObjectID::GetModuleIndex() const {
  ObjectIDModuleIndexType index;
  std::memcpy(&index, id_ + JobRunID::kLength + kModuleTypeBytesLength, sizeof(index));
  return index;
}

ObjectIDPaketIndexType ObjectID::GetPaketIndex() const {
  ObjectIDPaketIndexType index;
  std::memcpy(&index, id_ + JobRunID::kLength + kModuleTypeBytesLength + kModuleIndexBytesLength, sizeof(index));
  return index;
}

ObjectID ObjectID::GenerateObjectId(const std::string &run_id_binary,
                                    ObjectIDModuleType module_type,
                                    ObjectIDModuleIndexType module_index,
                                    ObjectIDPaketIndexType paket_index) {
  CHECK(run_id_binary.size() == JobRunID::Size());
  ObjectID ret = ObjectID::Nil();
  std::memcpy(ret.id_, run_id_binary.c_str(), JobRunID::kLength);
  std::memcpy(ret.id_ + JobRunID::kLength, &module_type, sizeof(module_type));
  std::memcpy(ret.id_ + JobRunID::kLength + kModuleTypeBytesLength, &module_index,
              sizeof(module_index));
  std::memcpy(ret.id_ + JobRunID::kLength + kModuleTypeBytesLength + kModuleIndexBytesLength, &paket_index,
              sizeof(paket_index));
  return ret;
}

JobRunID JobRunID::FromUniqueBytes(const JobRunID &job_id, const ObjectID &parent_task_id,
        const size_t parent_task_counter) {
  auto data = GenerateUniqueBytes(job_id, parent_task_id, parent_task_counter,
		  kLength);
  std::copy_n(job_id.Data(), kLength, std::back_inserter(data));
  CHECK(data.size() == kLength);
  return JobRunID::FromBinary(data);
}

JobRunID JobRunID::FromInt(uint16_t value) {
  std::vector<uint8_t> data(JobRunID::Size(), 0);
  std::memcpy(data.data(), &value, JobRunID::Size());
  return JobRunID::FromBinary(
      std::string(reinterpret_cast<const char *>(data.data()), data.size()));
}

#define ID_OSTREAM_OPERATOR(id_type)                              \
  std::ostream &operator<<(std::ostream &os, const id_type &id) { \
    if (id.IsNil()) {                                             \
      os << "NIL_ID";                                             \
    } else {                                                      \
      os << id.Hex();                                             \
    }                                                             \
    return os;                                                    \
  }

ID_OSTREAM_OPERATOR(UniqueID);
ID_OSTREAM_OPERATOR(JobRunID);
ID_OSTREAM_OPERATOR(ObjectID);

}  // namespace pl_proc
