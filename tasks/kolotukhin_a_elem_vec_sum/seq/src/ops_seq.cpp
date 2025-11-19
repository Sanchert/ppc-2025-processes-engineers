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
  return GetOutput() == 0;
}

bool KolotukhinAElemVecSumSEQ::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool KolotukhinAElemVecSumSEQ::RunImpl() {
  const std::vector<std::uint64_t> &input_vec = GetInput();
  volatile std::uint64_t total_sum = 0;
  volatile std::uint64_t vec_size = input_vec.size();
  for (std::uint64_t i = 0; i < vec_size; i++) {
    total_sum += input_vec[i];
  }
  GetOutput() = total_sum;
  return true;
}

bool KolotukhinAElemVecSumSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
