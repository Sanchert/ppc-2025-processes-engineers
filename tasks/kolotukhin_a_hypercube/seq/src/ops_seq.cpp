#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <numeric>
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
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  volatile std::atomic<double> compute_load{0.0};
  double temp = 0.0;
  double val = 0.0;
  for (int iter = 0; iter < 100000; iter++) {
    val = static_cast<double>(iter);
    compute_load.store(compute_load.load() + std::sin(val * 0.0001) * std::cos(val * 0.0001));
    if (iter % 1000 == 0) {
      temp = compute_load.load();
      compute_load.store(std::fmod(temp, 1000.0));
    }
  }
  double final_result = compute_load.load();
  (void)final_result;
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
