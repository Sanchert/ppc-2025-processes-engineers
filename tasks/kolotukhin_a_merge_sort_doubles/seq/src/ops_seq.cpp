#include "kolotukhin_a_merge_sort_doubles/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "kolotukhin_a_merge_sort_doubles/common/include/common.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_merge_sort_doubles {

KolotukhinAMergeSortDoublesSEQ::KolotukhinAMergeSortDoublesSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::vector<double>();
}

bool KolotukhinAMergeSortDoublesSEQ::ValidationImpl() {
  return true;
}

bool KolotukhinAMergeSortDoublesSEQ::PreProcessingImpl() {
  GetOutput().clear();
  GetOutput().resize(GetInput().size());
  return true;
}

bool KolotukhinAMergeSortDoublesSEQ::RunImpl() {
  const auto& input = GetInput();
  
  if (input.empty()) {
    GetOutput() = input;  // Пустой массив
    return true;
  }
  
  // Копируем входные данные
  std::vector<double> data = input;
  
  // Поразрядная сортировка для double
  const size_t n = data.size();
  std::vector<uint64_t> keys(n);
  
  // Преобразование double -> uint64_t
  for (size_t i = 0; i < n; ++i) {
    uint64_t u;
    std::memcpy(&u, &data[i], sizeof(double));
    
    // Для корректного порядка отрицательных чисел
    if (u & 0x8000000000000000ULL) {
      u = ~u;  // Инвертируем для отрицательных
    } else {
      u |= 0x8000000000000000ULL;  // Устанавливаем бит для положительных
    }
    keys[i] = u;
  }
  
  // Поразрядная сортировка
  const int RADIX_BITS = 8;
  const int RADIX_SIZE = 256;  // 2^8
  std::vector<uint64_t> temp(n);
  
  for (int shift = 0; shift < 64; shift += RADIX_BITS) {
    size_t count[RADIX_SIZE + 1] = {0};
    
    for (size_t i = 0; i < n; ++i) {
      uint8_t digit = static_cast<uint8_t>((keys[i] >> shift) & 0xFF);
      ++count[digit + 1];
    }
    
    for (int i = 0; i < RADIX_SIZE; ++i) {
      count[i + 1] += count[i];
    }
    
    for (size_t i = 0; i < n; ++i) {
      uint8_t digit = static_cast<uint8_t>((keys[i] >> shift) & 0xFF);
      temp[count[digit]++] = keys[i];
    }
    
    std::copy(temp.begin(), temp.end(), keys.begin());
  }
  
  // Обратное преобразование
  for (size_t i = 0; i < n; ++i) {
    uint64_t u = keys[i];
    
    if (u & 0x8000000000000000ULL) {
      u &= ~0x8000000000000000ULL;
    } else {
      u = ~u;
    }
    
    std::memcpy(&data[i], &u, sizeof(double));
  }
  
  // Записываем результат
  GetOutput() = data;
  
  return true;
}

bool KolotukhinAMergeSortDoublesSEQ::PostProcessingImpl() {
  // Проверяем, что массив отсортирован (для отладки)
  // В рабочей реализации эту проверку можно убрать
  const auto& output = GetOutput();
  for (size_t i = 1; i < output.size(); ++i) {
    if (output[i] < output[i - 1]) {
      return false;  // Массив не отсортирован
    }
  }

  return true;
}

}  // namespace kolotukhin_a_merge_sort_doubles
