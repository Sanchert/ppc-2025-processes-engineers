#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace kolotukhin_a_elem_vec_sum {

using InType = std::vector<uint64_t>;
using OutType = uint64_t;
using TestType = uint64_t;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_elem_vec_sum
