#include <gtest/gtest.h>

#include <cstddef>

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "kolotukhin_a_hypercube/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kolotukhin_a_hypercube {

class KolotukhinAHypercubePerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  int kDataSize_ = 400;
  InType input_data_{0, 0, 0};
  void SetUp() override {
    input_data_.source = 0;
    input_data_.data_size = kDataSize_;
    input_data_.dest = -2;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == kDataSize_;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KolotukhinAHypercubePerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, KolotukhinAHypercubeMPI, KolotukhinAHypercubeSEQ>(
    PPC_SETTINGS_kolotukhin_a_hypercube);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KolotukhinAHypercubePerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KolotukhinAHypercubePerfTests, kGtestValues, kPerfTestName);

}  // namespace kolotukhin_a_hypercube
