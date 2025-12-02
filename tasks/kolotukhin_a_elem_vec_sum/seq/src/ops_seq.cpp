#include "kolotukhin_a_elem_vec_sum/seq/include/ops_seq.hpp"

#include <cstdint>
#include <vector>

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"

namespace kolotukhin_a_elem_vec_sum {

KolotukhinAElemVecSumSEQ::KolotukhinAElemVecSumSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KolotukhinAElemVecSumSEQ::ValidationImpl() {
  if (!std::equal_to<>()(typeid(GetInput()), typeid(std::uint64_t))) {
    return false;
  }
  return true;
}

bool KolotukhinAElemVecSumSEQ::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool KolotukhinAElemVecSumSEQ::RunImpl() {
  std::uint64_t size = GetInput();
  if (size == 0) {
    GetOutput() = 0;
    return true;
  }

  std::int64_t total_sum = 0;
  for (std::uint64_t i = 0; i < size; i++) {
    total_sum += i % 256;
  }

  GetOutput() = total_sum;
  return true;
}

bool KolotukhinAElemVecSumSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
