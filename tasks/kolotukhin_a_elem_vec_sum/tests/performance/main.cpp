#include <gtest/gtest.h>
#include <mpi.h>

#include <cstdint>

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"
#include "kolotukhin_a_elem_vec_sum/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_elem_vec_sum/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kolotukhin_a_elem_vec_sum {

class KolotukhinAElemVecSumPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  std::uint64_t kCount_ = 100000000;
  std::int64_t test_answer_ = 500150000000;
  InType input_data_;

  void SetUp() override {
    int p_id = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_id);
    if (p_id == 0) {
      input_data_.resize(kCount_);
      int seed = 42;
      for (std::uint64_t i = 0; i < kCount_; i++) {
        seed = (seed * 13 + 7) % 10000;
        input_data_[i] = seed;
      }
    }
  }

  bool CheckTestOutputData(OutType &output_data) final {
    int p_id = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_id);
    if (p_id == 0) {
      return output_data == test_answer_;
    }
    return true;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KolotukhinAElemVecSumPerfTest, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, KolotukhinAElemVecSumMPI, KolotukhinAElemVecSumSEQ>(
    PPC_SETTINGS_kolotukhin_a_elem_vec_sum);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KolotukhinAElemVecSumPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KolotukhinAElemVecSumPerfTest, kGtestValues, kPerfTestName);

}  // namespace kolotukhin_a_elem_vec_sum
