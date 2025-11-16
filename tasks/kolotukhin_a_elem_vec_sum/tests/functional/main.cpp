#include <gtest/gtest.h>
#include <stb/stb_image.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "kolotukhin_a_elem_vec_sum/common/include/common.hpp"
#include "kolotukhin_a_elem_vec_sum/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_elem_vec_sum/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_elem_vec_sum {

class KolotukhinAElemVecSumFuncTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return std::to_string(test_param);
  }

 protected:
  void SetUp() override {
    TestType n = std::get<static_cast<int>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    input_data_.resize(n);

    for (std::size_t i = 0; i < n; i++) {
      input_data_[i] = i;
    }

    sum_ = (n * n - n) / 2;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return sum_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType sum_;
};

namespace {

TEST_P(KolotukhinAElemVecSumFuncTests, MatmulFromPic) {
  ExecuteTest(GetParam());
}
const std::array<TestType, 3> kTestParam = {10, 500, 10000};
const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KolotukhinAElemVecSumMPI, InType>(kTestParam, PPC_SETTINGS_kolotukhin_a_elem_vec_sum),
    ppc::util::AddFuncTask<KolotukhinAElemVecSumSEQ, InType>(kTestParam, PPC_SETTINGS_kolotukhin_a_elem_vec_sum));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KolotukhinAElemVecSumFuncTests::PrintFuncTestName<KolotukhinAElemVecSumFuncTests>;

INSTANTIATE_TEST_SUITE_P(PicMatrixTests, KolotukhinAElemVecSumFuncTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace kolotukhin_a_elem_vec_sum
