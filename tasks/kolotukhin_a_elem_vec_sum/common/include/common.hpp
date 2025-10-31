#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace kolotukhin_a_elem_vec_sum {

using InType = std::vector<int>;
using OutType = int;
using TestType = int;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_elem_vec_sum
