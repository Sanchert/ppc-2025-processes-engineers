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
  const std::vector<int> &inputVec = GetInput();

  int pid;
  int pCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &pCount);

  int elemsCount = inputVec.size();

  std::vector<int> sizes(pCount);
  std::vector<int> startingPoints(pCount);

  if (pid == 0) {
    int baseSize = elemsCount / pCount;
    int remainder = elemsCount % pCount;
    int step = 0;
    for (int pRank = 0; pRank < pCount; pRank++) {
      sizes[pRank] = baseSize + (pRank < remainder ? 1 : 0);
      startingPoints[pRank] = step;
      step += sizes[pRank];
    }
  }

  MPI_Bcast(sizes.data(), pCount, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(startingPoints.data(), pCount, MPI_INT, 0, MPI_COMM_WORLD);
  
  int partSize = sizes[pid];
  std::vector<int> local_data(partSize);

  MPI_Scatterv(inputVec.data(), sizes.data(), startingPoints.data(), MPI_INT, local_data.data(), partSize, MPI_INT, 0,
               MPI_COMM_WORLD);

  int local_sum = std::accumulate(local_data.begin(), local_data.end(), 0);
  int global_sum = 0;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  int sum = 0;
  if (pid == 0) {
    sum = global_sum;
  }

  MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

  GetOutput() = sum;

  return true;
}

bool KolotukhinAElemVecSumMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_elem_vec_sum
