#include <gtest/gtest.h>
#include <mpi.h>

#include <cstddef>

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "kolotukhin_a_hypercube/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kolotukhin_a_hypercube {

class KolotukhinAHypercubePerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  std::size_t kDataSize_ = 400;
  InType input_data_{0, 0, 0};
  void SetUp() override {
    input_data_.source = 0;
    input_data_.data_size = kDataSize_;
    input_data_.dest = -2;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    if (output_data.exec) {
      if (output_data.process_id == input_data_.dest) {
        if (output_data.data.size() != kDataSize_) {
          return false;
        }
        for (std::size_t i = 0; i < kDataSize_; i++) {
          if (output_data.data[i] != (static_cast<int>(i) * 2) + 1) {
            return false;
          }
        }
      }
    }
    return true;
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
