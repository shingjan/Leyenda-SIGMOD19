#include "radix_partition.h"

#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int kTupleSize = 100;
const int kKeySize = 10;

void radixPartition(const std::string &infile, const std::string &outfile) {
  std::ifstream is(infile);
  if (!is) {
    std::cerr << "Could not open the file\n";
    std::exit(-1);
  }

  // get size of file
  is.seekg(0, is.end);
  const std::int64_t size = is.tellg();
  is.seekg(0);

  const std::int64_t num_tuples = size / kTupleSize;

  const std::uint16_t kNumPartitions = 1 << kNumPartitionBits;
  std::vector<int> histogram(kNumPartitions);
  char buffer[kTupleSize];

  // read content of is
  for (std::int64_t i = 0; i < num_tuples; ++i) {
    is.read(buffer, kTupleSize);
    const std::uint16_t p = getPartitionId(buffer);
    ++histogram[p];
  }

  std::vector<int> offset(kNumPartitions);
  int count = histogram[0];
  for (std::uint16_t p = 1; p < kNumPartitions; ++p) {
    offset[p] = count;
    count += histogram[p];
  }

  std::vector<char> output(size);

  // read content of is
  is.seekg(0);
  for (std::int64_t i = 0; i < num_tuples; ++i) {
    is.read(buffer, kTupleSize);
    const std::uint16_t p = getPartitionId(buffer);
    const std::uint64_t dest = offset[p]++;
    std::memcpy(&output[dest * kTupleSize], buffer, kTupleSize);
  }
  is.close();

  std::ofstream os(outfile);
  os.write(output.data(), size);
  os.close();
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "USAGE: " << argv[0] << " [in-file] [outfile]\n";
    std::exit(-1);
  }

  radixPartition(argv[1], argv[2]);

  return 0;
}

