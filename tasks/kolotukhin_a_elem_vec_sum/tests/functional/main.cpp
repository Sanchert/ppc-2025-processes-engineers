#include <gtest/gtest.h>
#include <stb/stb_image.h>
#include <mpi.h>

#include <array>
#include <cstdint>
#include <string>

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
    int p_id = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_id);
    if (p_id == 0) {
      std::uint64_t size = std::get<static_cast<std::uint64_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
      input_data_.resize(size);
      for (std::uint64_t i = 0; i < size; i++) {
        input_data_[i] = i  % 256;
      }
    }
  }

  bool CheckTestOutputData(OutType &output_data) final {
    int pid = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    if (pid == 0) {
      std::uint64_t n = std::get<static_cast<std::uint64_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());

      std::int64_t full_cycles = n / 256;
      std::int64_t remainder = n % 256;

      std::int64_t sum_full_cycles = full_cycles * 32640;
      std::int64_t sum_remainder = (remainder * (remainder - 1)) / 2;
      return output_data == sum_full_cycles + sum_remainder;
    }
    return true;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
};

namespace {

TEST_P(KolotukhinAElemVecSumFuncTests, MatmulFromPic) {
  ExecuteTest(GetParam());
}
const std::array<TestType, 4> kTestParam = {1, 20, 123, 10000};

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KolotukhinAElemVecSumMPI, InType>(kTestParam, PPC_SETTINGS_kolotukhin_a_elem_vec_sum),
    ppc::util::AddFuncTask<KolotukhinAElemVecSumSEQ, InType>(kTestParam, PPC_SETTINGS_kolotukhin_a_elem_vec_sum));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KolotukhinAElemVecSumFuncTests::PrintFuncTestName<KolotukhinAElemVecSumFuncTests>;

INSTANTIATE_TEST_SUITE_P(PicMatrixTests, KolotukhinAElemVecSumFuncTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace kolotukhin_a_elem_vec_sum
