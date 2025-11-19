#include "kolotukhin_a_elem_vec_sum/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <cstdint>
#include <vector>

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_elem_vec_sum {

KolotukhinAElemVecSumMPI::KolotukhinAElemVecSumMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KolotukhinAElemVecSumMPI::ValidationImpl() {
  return GetOutput() == 0;
}

bool KolotukhinAElemVecSumMPI::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool KolotukhinAElemVecSumMPI::RunImpl() {
  int pid = 0;
  int p_count = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &p_count);

  const std::vector<std::uint64_t> &input_vec = GetInput();
  std::uint64_t uint_pid = static_cast<std::uint64_t>(pid);
  std::uint64_t uint_p_count = static_cast<std::uint64_t>(p_count);
  std::uint64_t vector_size = input_vec.size();
  std::uint64_t min_part = vector_size / uint_p_count;
  std::uint64_t proc_size = min_part + (std::cmp_less(uint_pid, vector_size % uint_p_count) ? 1 : 0);
  std::uint64_t rem = vector_size % uint_p_count;
  std::uint64_t local_sum = 0;
  std::uint64_t start = (min_part * uint_pid) + (std::cmp_less(uint_pid, rem) ? uint_pid : rem);
  std::uint64_t end = start + proc_size;
  for (std::uint64_t i = start; (i < end) && (i < input_vec.size()); i++) {
    local_sum += input_vec[i];
  }
  std::uint64_t global_sum = 0;

  MPI_Allreduce(&local_sum, &global_sum, 1, MPI_UINT64_T, MPI_SUM, MPI_COMM_WORLD);
  GetOutput() = global_sum;
  return true;
}

bool KolotukhinAElemVecSumMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
