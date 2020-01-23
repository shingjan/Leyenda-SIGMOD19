#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int kTupleSize = 100;
const int kKeySize = 10;

void sort(const std::string &infile, const std::string &outfile) {
  std::ifstream is(infile);
  if (!is) {
    std::cerr << "Could not open the file\n";
    std::exit(-1);
  }

  // get size of file
  is.seekg(0, is.end);
  const std::int64_t size = is.tellg();
  is.seekg(0);

  // allocate memory for file content
  const std::int64_t num_tuples = size / kTupleSize;
  std::vector<std::array<char, kTupleSize>> buffer(num_tuples);

  std::ofstream os(outfile);

  // read content of is
  for (std::int64_t i = 0; i < num_tuples; ++i) {
    is.read(buffer[i].data(), kTupleSize);
  }

  std::sort(buffer.begin(), buffer.end(),
            [](const std::array<char, kTupleSize> &lhs,
               const std::array<char, kTupleSize> &rhs) {
              for (int i = 0; i < kKeySize; ++i) {
                if (lhs[i] == rhs[i]) {
                  continue;
                }
                return unsigned(lhs[i]) < unsigned(rhs[i]);
              }

              return false;
            });

  for (std::int64_t i = 0; i < num_tuples; ++i) {
    os.write(buffer[i].data(), kTupleSize);
  }
  is.close();
  os.close();
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "USAGE: " << argv[0] << " [in-file] [outfile]\n";
    std::exit(-1);
  }

  sort(argv[1], argv[2]);

  return 0;
}

