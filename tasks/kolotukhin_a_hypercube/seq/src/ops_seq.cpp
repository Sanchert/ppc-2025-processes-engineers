#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include <vector>

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_hypercube {

KolotukhinAHypercubeSEQ::KolotukhinAHypercubeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput().data = std::vector<int>{};
  GetOutput().process_id = 0;
  GetOutput().exec = false;
}

bool KolotukhinAHypercubeSEQ::ValidationImpl() {
  return true;
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  GetOutput().data.clear();
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
