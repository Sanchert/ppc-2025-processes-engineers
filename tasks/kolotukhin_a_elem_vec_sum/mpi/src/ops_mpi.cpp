#include "kolotukhin_a_elem_vec_sum/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <cstdint>
#include <vector>

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"

namespace kolotukhin_a_elem_vec_sum {

KolotukhinAElemVecSumMPI::KolotukhinAElemVecSumMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KolotukhinAElemVecSumMPI::ValidationImpl() {
  if (!std::equal_to<>()(typeid(GetInput()), typeid(std::uint64_t))) {
    return false;
  }
  return true;
}

bool KolotukhinAElemVecSumMPI::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool KolotukhinAElemVecSumMPI::RunImpl() {
  int p_id = -1;
  int p_count = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &p_id);
  MPI_Comm_size(MPI_COMM_WORLD, &p_count);

  std::size_t input_size = GetInput();
  if (input_size == 0) {
    GetInput() = 0;
    return true;
  }
  auto uint_pid = static_cast<std::uint64_t>(p_id);
  auto uint_p_count = static_cast<std::uint64_t>(p_count);

  std::uint64_t min_part = input_size / uint_p_count;
  std::uint64_t rem = input_size % uint_p_count;
  std::uint64_t proc_size = min_part + (std::less<>()(uint_pid, rem) ? 1 : 0);
  std::int64_t local_sum = 0;
  std::uint64_t start = (min_part * uint_pid) + (std::less<>()(uint_pid, rem) ? uint_pid : rem);
  std::uint64_t end = start + proc_size;
  for (std::uint64_t i = start; (i < end); i++) {
    local_sum += i % 256;
  }

  std::int64_t global_sum = 0;
  MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT64_T, MPI_SUM, MPI_COMM_WORLD);

  GetOutput() = global_sum;
  return true;
}

bool KolotukhinAElemVecSumMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
