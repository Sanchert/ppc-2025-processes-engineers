#pragma once

#include "task/include/task.hpp"

#include <cstdint>

namespace kolotukhin_a_elem_vec_sum {

using InType = std::vector<uint64_t>;
using OutType = std::uint64_t;
using TestType = std::uint64_t;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_elem_vec_sum
