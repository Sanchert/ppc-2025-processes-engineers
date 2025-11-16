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
  
  const std::vector<int> &inputVec = GetInput();
  int vectorSize = inputVec.size();
  int minPart = vectorSize / pCount;
  int procSize = minPart + (pid < vectorSize % pCount ? 1 : 0);
  
  int local_sum = 0;
  size_t start = minPart * pid + (pid < vectorSize % pCount ? pid : vectorSize % pCount);
  size_t end = start + procSize;
  for (size_t i = start; i < end && i < inputVec.size(); i++) {
    local_sum += inputVec[i];
  }
  int global_sum;

  MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
  GetOutput() = global_sum;
  return true;
}

bool KolotukhinAElemVecSumMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
