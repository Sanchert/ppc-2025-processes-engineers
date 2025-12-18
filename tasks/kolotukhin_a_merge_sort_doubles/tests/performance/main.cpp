#include <gtest/gtest.h>

#include "kolotukhin_a_merge_sort_doubles/common/include/common.hpp"
#include "kolotukhin_a_merge_sort_doubles/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_merge_sort_doubles/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kolotukhin_a_merge_sort_doubles {

class KolotukhinAMergeSortDoublesPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};

  void SetUp() override {
    input_data_ = kCount_;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return input_data_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KolotukhinAMergeSortDoublesPerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KolotukhinAMergeSortDoublesMPI, KolotukhinAMergeSortDoublesSEQ>(PPC_SETTINGS_kolotukhin_a_merge_sort_doubles);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KolotukhinAMergeSortDoublesPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KolotukhinAMergeSortDoublesPerfTests, kGtestValues, kPerfTestName);

}  // namespace kolotukhin_a_merge_sort_doubles
