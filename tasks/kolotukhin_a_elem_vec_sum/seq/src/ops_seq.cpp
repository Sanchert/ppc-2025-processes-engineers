#include "kolotukhin_a_elem_vec_sum/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"
#include "util/include/util.hpp"

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
  const std::vector<uint64_t> &inputVec = GetInput();
  volatile uint64_t sum = 0;
  volatile uint64_t s = inputVec.size();
  for (uint64_t i = 0; i < s; i++) {
    sum += inputVec[i];
  }
  GetOutput() = sum;
  return true;
}

bool KolotukhinAElemVecSumSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
