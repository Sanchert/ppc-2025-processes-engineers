#include <gtest/gtest.h>
#include <stb/stb_image.h>

#include <array>
#include <cstdint>
#include <string>
#include <tuple>

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "kolotukhin_a_hypercube/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_hypercube {

class KolotukhinAHypercubeFuncTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return test_param.name;
  }

 protected:
  void SetUp() override {
    TestType tt = std::get<static_cast<std::uint8_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    input_data_.source = tt.msg.source;
    input_data_.dest = tt.msg.dest;
    input_data_.data_size = tt.msg.data_size;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    TestType tt = std::get<static_cast<std::uint8_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    return output_data == tt.msg.data_size;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_{};
};

namespace {

TEST_P(KolotukhinAHypercubeFuncTests, MatmulFromPic) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 6> kTestParam = {TestType(HypercubeMessage(0, 3, 4), {1, 1, 1, 1}, "far"),
                                            TestType(HypercubeMessage(0, 3, 0), {}, "far_empty"),
                                            TestType(HypercubeMessage(0, 1, 4), {1, 1, 1, 1}, "neighboors"),
                                            TestType(HypercubeMessage(2, 3, 4), {1, 1, 1, 1}, "zero_not_source"),
                                            TestType(HypercubeMessage(2, 2, 4), {1, 1, 1, 1}, "to_itself"),
                                            TestType(HypercubeMessage(3, 0, 4), {1, 1, 1, 1}, "reverse")};

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KolotukhinAHypercubeMPI, InType>(kTestParam, PPC_SETTINGS_kolotukhin_a_hypercube),
    ppc::util::AddFuncTask<KolotukhinAHypercubeSEQ, InType>(kTestParam, PPC_SETTINGS_kolotukhin_a_hypercube));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KolotukhinAHypercubeFuncTests::PrintFuncTestName<KolotukhinAHypercubeFuncTests>;

INSTANTIATE_TEST_SUITE_P(PicMatrixTests, KolotukhinAHypercubeFuncTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace kolotukhin_a_hypercube
