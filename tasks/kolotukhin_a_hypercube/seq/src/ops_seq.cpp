#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include <cstddef>
#include <vector>

#include "kolotukhin_a_hypercube/common/include/common.hpp"

namespace kolotukhin_a_hypercube {

KolotukhinAHypercubeSEQ::KolotukhinAHypercubeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KolotukhinAHypercubeSEQ::ValidationImpl() {
  return GetOutput() == 0;
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  GetOutput() = GetInput().data_size;
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
