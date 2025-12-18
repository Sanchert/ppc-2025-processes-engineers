#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace kolotukhin_a_merge_sort_doubles {

using InType = int;
using OutType = int;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kolotukhin_a_merge_sort_doubles
