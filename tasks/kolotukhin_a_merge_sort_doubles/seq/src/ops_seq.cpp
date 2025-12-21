#include "kolotukhin_a_merge_sort_doubles/seq/include/ops_seq.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "kolotukhin_a_merge_sort_doubles/common/include/common.hpp"

namespace kolotukhin_a_merge_sort_doubles {

KolotukhinAMergeSortDoublesSEQ::KolotukhinAMergeSortDoublesSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::vector<double>();
}

bool KolotukhinAMergeSortDoublesSEQ::ValidationImpl() {
  return true;
}

bool KolotukhinAMergeSortDoublesSEQ::PreProcessingImpl() {
  GetOutput().clear();
  return true;
}

bool KolotukhinAMergeSortDoublesSEQ::RunImpl() {
  const auto &input = GetInput();
  GetOutput().resize(GetInput().size());
  if (input.empty()) {
    GetOutput() = input;
    return true;
  }
  std::vector<double> data = input;
  const std::size_t data_size = data.size();
  std::vector<std::uint64_t> keys(data_size);
  for (std::size_t i = 0; i < data_size; i++) {
    std::uint64_t u;
    std::memcpy(&u, &data[i], sizeof(double));
    if (u & 0x8000000000000000ULL) {
      u = ~u;
    } else {
      u |= 0x8000000000000000ULL;
    }
    keys[i] = u;
  }
  const int RADIX_BITS = 8;
  const int RADIX_SIZE = 256;
  std::vector<std::uint64_t> temp(data_size);
  for (int shift = 0; shift < 64; shift += RADIX_BITS) {
    std::size_t count[RADIX_SIZE + 1] = {0};
    for (std::size_t i = 0; i < data_size; ++i) {
      std::uint8_t digit = static_cast<std::uint8_t>((keys[i] >> shift) & 0xFF);
      ++count[digit + 1];
    }
    for (int i = 0; i < RADIX_SIZE; ++i) {
      count[i + 1] += count[i];
    }
    for (std::size_t i = 0; i < data_size; ++i) {
      std::uint8_t digit = static_cast<std::uint8_t>((keys[i] >> shift) & 0xFF);
      temp[count[digit]++] = keys[i];
    }
    std::copy(temp.begin(), temp.end(), keys.begin());
  }
  for (std::size_t i = 0; i < data_size; ++i) {
    std::uint64_t u = keys[i];
    if (u & 0x8000000000000000ULL) {
      u &= ~0x8000000000000000ULL;
    } else {
      u = ~u;
    }
    std::memcpy(&data[i], &u, sizeof(double));
  }
  GetOutput() = data;
  return true;
}

bool KolotukhinAMergeSortDoublesSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_merge_sort_doubles
