#pragma once

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"
#include "task/include/task.hpp"

namespace kolotukhin_a_elem_vec_sum {

class KolotukhinAElemVecSumMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit KolotukhinAElemVecSumMPI(const InType &in);

 private:
  bool ValidationImpl() override;      // адекватность входных данных
  bool PreProcessingImpl() override;   // предварительная обработка входных данных
  bool RunImpl() override;             // реализация mpi/др.вид параллелизма
  bool PostProcessingImpl() override;  // данные переводятся в удобочитаемый формат
};

}  // namespace kolotukhin_a_elem_vec_sum
