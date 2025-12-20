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
  return true;
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  GetOutput().data.clear();
  GetOutput().exec = true;
  GetOutput().process_id = -1;
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  std::size_t data_size = GetInput().data_size;
  std::vector<int> data_to_send{};
  for (std::size_t i = 0; i < data_size; i++) {
    data_to_send.push_back((static_cast<int>(i) * 2) + 1);
  }
  GetOutput().data = data_to_send;
  GetOutput().process_id = GetInput().dest;
  data_to_send.clear();
  // std::cout << "[PROCESS] #" << GetOutput().data.size() << std::endl;
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
