#include <gtest/gtest.h>

#include "kolotukhin_a_merge_sort_doubles/common/include/common.hpp"
#include "kolotukhin_a_merge_sort_doubles/mpi/include/ops_mpi.hpp"
#include "kolotukhin_a_merge_sort_doubles/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kolotukhin_a_merge_sort_doubles {

class KolotukhinAMergeSortDoublesPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  void SetUp() override {
    GenerateTestData();
  }

  bool CheckTestOutputData(OutType &output_data) final {
    // Простая проверка сортировки
    for (size_t i = 1; i < output_data.size(); ++i) {
      if (output_data[i] < output_data[i - 1]) {
        return false;
      }
    }
    return true;
  }

  InType GetTestInputData() final {
    return test_data_;
  }
  private:
  void GenerateTestData() {
    const size_t data_size = 1000000;  // 1 миллион элементов
    
    test_data_.resize(data_size);
    
    // Заполняем массив числами в обратном порядке
    // Это создает интересный случай для сортировки
    for (size_t i = 0; i < data_size; ++i) {
      // Используем простую формулу для генерации чисел
      // Чередуем положительные и отрицательные числа
      double value = static_cast<double>(data_size - i);
      
      // Каждое 3-е число делаем отрицательным
      if (i % 3 == 0) {
        value = -value;
      }
      
      // Каждое 7-е число делаем дробным
      if (i % 7 == 0) {
        value += 0.5;
      }
      
      test_data_[i] = value;
    }
  }
  
  InType test_data_;
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
