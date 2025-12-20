#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include "kolotukhin_a_hypercube/common/include/common.hpp"

namespace kolotukhin_a_hypercube {

KolotukhinAHypercubeSEQ::KolotukhinAHypercubeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = Out{};
  GetOutput().exec = false;
}

bool KolotukhinAHypercubeSEQ::ValidationImpl() {
  return true;
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
