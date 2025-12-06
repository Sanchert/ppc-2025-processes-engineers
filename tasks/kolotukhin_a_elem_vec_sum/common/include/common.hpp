#pragma once

#include <cstdint>
#include <vector>

#include "task/include/task.hpp"

namespace kolotukhin_a_elem_vec_sum {

using InType = std::vector<int>;
using OutType = std::int64_t;
using TestType = std::tuple<std::uint64_t, std::int64_t>;  // размер, результат
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_elem_vec_sum
