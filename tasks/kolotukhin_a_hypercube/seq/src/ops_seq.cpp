#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include <cstddef>
#include <vector>

#include "kolotukhin_a_hypercube/common/include/common.hpp"

namespace kolotukhin_a_hypercube {

KolotukhinAHypercubeSEQ::KolotukhinAHypercubeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = Out{};
}

bool KolotukhinAHypercubeSEQ::ValidationImpl() {
  return GetOutput().data.empty();
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  GetOutput().data.clear();
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  std::size_t data_size = GetInput().data_size;
  if (GetOutput().data.empty()) {
    GetOutput().data.resize(data_size, 1);
  }
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  GetOutput().process_id = GetInput().dest;
  return true;
}

}  // namespace kolotukhin_a_hypercube
