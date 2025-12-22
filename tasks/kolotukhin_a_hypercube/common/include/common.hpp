#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "task/include/task.hpp"

namespace kolotukhin_a_hypercube {

// struct HypercubeMessage {
//   int source = 0;
//   int dest = 0;
//   int data_size = 0;
// };

// struct TT {
//   HypercubeMessage msg;
//   std::string name;
// };

using InType = std::vector<int>;
using OutType = int;
using TestType = std::tuple<std::vector<int>, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_hypercube
