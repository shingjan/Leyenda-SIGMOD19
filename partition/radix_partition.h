#ifndef RADIX_PARTITION_H_
#define RADIX_PARTITION_H_

#include <cstdint>

// 2^10 partitions.
const std::uint16_t kNumPartitionBits = 10u;
constexpr std::uint16_t kShift = kNumPartitionBits - sizeof(char) * 8;
constexpr std::uint16_t kLowMask = (1u << kShift) - 1u;
constexpr std::uint16_t kMask = (1u << kNumPartitionBits) - 1u;

std::uint16_t getPartitionId(const char *key) {
  std::uint16_t pid = (*key) << kShift;
  pid += *(key + 1) & kLowMask;
  pid &= kMask;
  return pid;
}

#endif  // RADIX_PARTITION_H_

