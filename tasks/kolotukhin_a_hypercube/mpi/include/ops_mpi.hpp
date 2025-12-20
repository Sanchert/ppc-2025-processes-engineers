#pragma once

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "task/include/task.hpp"

namespace kolotukhin_a_hypercube {

class KolotukhinAHypercubeMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit KolotukhinAHypercubeMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

  static int CalculateHypercubeDimension(int num_proc);
  static int GetNeighbor(int rank, int dim);
  static void PerformComputeLoad(int iterations);

  bool exec_ = true;
};

}  // namespace kolotukhin_a_hypercube
