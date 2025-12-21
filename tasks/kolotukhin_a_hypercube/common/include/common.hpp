#pragma once

#include <string>
#include <vector>

#include "task/include/task.hpp"

namespace kolotukhin_a_hypercube {

struct HypercubeMessage {
  int source = 0;
  int dest = 0;
  int data_size = 0;
};

struct TT {
  HypercubeMessage msg;
  std::vector<int> data;
  std::string name;
};

using InType = HypercubeMessage;
using OutType = int;
using TestType = TT;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_hypercube
