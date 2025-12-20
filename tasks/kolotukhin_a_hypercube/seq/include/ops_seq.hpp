#pragma once
#include <mpi.h>

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "task/include/task.hpp"

namespace kolotukhin_a_hypercube {

class KolotukhinAHypercubeSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit KolotukhinAHypercubeSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

  int CalculateHypercubeDimension(int num_processes);
  void PerformComputeLoad(int iterations);
  MPI_Comm CreateHypercubeComm(int dims);
  MPI_Comm hypercube_comm_ = MPI_COMM_NULL;
  bool valid_ = true;
};

}  // namespace kolotukhin_a_hypercube
