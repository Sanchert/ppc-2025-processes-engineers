#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "task/include/task.hpp"

namespace kolotukhin_a_hypercube {

struct HypercubeMessage {
  int source = 0;
  int dest = 0;
  std::size_t data_size = 0;
};

struct TT {
  HypercubeMessage msg;
  std::vector<int> data;
  std::string name;
};

using InType = HypercubeMessage;
using OutType = std::tuple<std::vector<int>, bool>;
using TestType = TT;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_hypercube
