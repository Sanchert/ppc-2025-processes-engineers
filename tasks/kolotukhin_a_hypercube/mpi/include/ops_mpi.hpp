#pragma once

#include <vector>

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

  int CalculateHypercubeDimension(int num_proc);
  int GetNeighbor(int rank, int dim);
  void PerformComputeLoad(int iterations);
  void SendData(std::vector<int> &data, int next_neighbor);
  void RecvData(std::vector<int> &data, int prev_neighbor);
  void CalcPositions(int my_rank, std::vector<int> &path, int &my_pos, int &next, int &prev);
  std::vector<int> CalcPath(int source, int dest, int dimensions);
};

}  // namespace kolotukhin_a_hypercube
