#include "kolotukhin_a_merge_sort_doubles/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "kolotukhin_a_merge_sort_doubles/common/include/common.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_merge_sort_doubles {

namespace {
  // Вспомогательные функции для поразрядной сортировки
  void radix_sort_doubles(std::vector<double>& data) {
    const size_t n = data.size();
    if (n <= 1) return;
    
    std::vector<uint64_t> keys(n);
    
    // Преобразование double -> uint64_t
    for (size_t i = 0; i < n; ++i) {
      uint64_t u;
      std::memcpy(&u, &data[i], sizeof(double));
      
      if (u & 0x8000000000000000ULL) {
        u = ~u;
      } else {
        u |= 0x8000000000000000ULL;
      }
      keys[i] = u;
    }
    
    // Поразрядная сортировка
    const int RADIX_SIZE = 256;
    std::vector<uint64_t> temp(n);
    
    for (int shift = 0; shift < 64; shift += 8) {
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
  }
  
  std::vector<double> merge_sorted_arrays(const std::vector<double>& a, 
                                         const std::vector<double>& b) {
    std::vector<double> result;
    result.reserve(a.size() + b.size());
    
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
      if (a[i] <= b[j]) {
        result.push_back(a[i++]);
      } else {
        result.push_back(b[j++]);
      }
    }
    
    while (i < a.size()) result.push_back(a[i++]);
    while (j < b.size()) result.push_back(b[j++]);
    
    return result;
  }
}  // namespace

KolotukhinAMergeSortDoublesMPI::KolotukhinAMergeSortDoublesMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::vector<double>();
}

bool KolotukhinAMergeSortDoublesMPI::ValidationImpl() {
  int mpi_initialized = 0;
  MPI_Initialized(&mpi_initialized);
  return (mpi_initialized != 0);
}

bool KolotukhinAMergeSortDoublesMPI::PreProcessingImpl() {
  GetOutput().clear();
  GetOutput().resize(GetInput().size());
  return true;
}

bool KolotukhinAMergeSortDoublesMPI::RunImpl() {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const auto& input = GetInput();
  int global_size = static_cast<int>(input.size());
  MPI_Bcast(&global_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (global_size == 0) {
    GetOutput() = std::vector<double>();
    MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }


  int local_size = global_size / size;
  if (rank < global_size % size) {
    local_size++;
  }

  std::vector<int> displs(size, 0);
  std::vector<int> recv_counts(size, 0);

  if (rank == 0) {
    int offset = 0;
    for (int i = 0; i < size; ++i) {
      recv_counts[i] = global_size / size + (i < (global_size % size) ? 1 : 0);
      displs[i] = offset;
      offset += recv_counts[i];
    }
  }

  MPI_Bcast(recv_counts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(displs.data(), size, MPI_INT, 0, MPI_COMM_WORLD);


  std::vector<double> local_data(local_size);

  if (rank == 0) {

    std::copy(input.begin(), input.begin() + local_size, local_data.begin());


    for (int i = 1; i < size; ++i) {
      if (recv_counts[i] > 0) {
        MPI_Send(input.data() + displs[i], recv_counts[i], MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }
    }
  } else if (local_size > 0) {
    MPI_Recv(local_data.data(), local_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  radix_sort_doubles(local_data);

  int step = 1;
  while (step < size) {
    if (rank % (2 * step) == 0) {
      int source_rank = rank + step;
      if (source_rank < size) {
        int remote_size;
        MPI_Recv(&remote_size, 1, MPI_INT, source_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (remote_size > 0) {
          std::vector<double> remote_data(remote_size);
          MPI_Recv(remote_data.data(), remote_size, MPI_DOUBLE, source_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          local_data = merge_sorted_arrays(local_data, remote_data);
        }
      }
    } else if ((rank - step) % (2 * step) == 0) {
      int dest_rank = rank - step;
      int send_size = static_cast<int>(local_data.size());

      MPI_Send(&send_size, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
      if (send_size > 0) {
        MPI_Send(local_data.data(), send_size, MPI_DOUBLE, dest_rank, 1, MPI_COMM_WORLD);
      }
      local_data.clear();
    }

    step *= 2;
    MPI_Barrier(MPI_COMM_WORLD);
  }

  if (rank != 0) {
    local_data.resize(global_size);
  }
  MPI_Bcast(local_data.data(), global_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  GetOutput() = local_data;
  // std::cout << "[LOCAL SIZE] #" << rank << " is " << local_data.size() << std::endl;
  return true;
}

bool KolotukhinAMergeSortDoublesMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_merge_sort_doubles
