#include "kolotukhin_a_elem_vec_sum/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
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
  int pid;
  int pCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &pCount);
  
  const std::vector<uint64_t> &inputVec = GetInput();
  uint64_t vectorSize = inputVec.size();
  uint64_t minPart = vectorSize / pCount;
  uint64_t procSize = minPart + (pid < vectorSize % pCount ? 1 : 0);
  
  uint64_t local_sum = 0;
  uint64_t start = minPart * pid + (pid < vectorSize % pCount ? pid : vectorSize % pCount);
  uint64_t end = start + procSize;
  for (uint64_t i = start; i < end && i < inputVec.size(); i++) {
    local_sum += inputVec[i];
  }
  uint64_t global_sum;

  MPI_Allreduce(&local_sum, &global_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
  
  GetOutput() = global_sum;
  return true;
}

bool KolotukhinAElemVecSumMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
