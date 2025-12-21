#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include <cstddef>
#include <vector>

#include "kolotukhin_a_hypercube/common/include/common.hpp"

namespace kolotukhin_a_hypercube {

KolotukhinAHypercubeSEQ::KolotukhinAHypercubeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = {};
}

bool KolotukhinAHypercubeSEQ::ValidationImpl() {
  return std::get<0>(GetOutput()).empty();
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  std::get<0>(GetOutput()).clear();
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  std::size_t data_size = GetInput().data_size;
  if (std::get<0>(GetOutput()).empty()) {
    std::get<0>(GetOutput()).resize(data_size, 1);
  }
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
