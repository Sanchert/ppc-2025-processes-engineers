#include "kolotukhin_a_hypercube/seq/include/ops_seq.hpp"

#include <mpi.h>

#include <numeric>
#include <tuple>
#include <vector>

#include "kolotukhin_a_hypercube/common/include/common.hpp"
#include "util/include/util.hpp"

namespace kolotukhin_a_hypercube {

MPI_Comm KolotukhinAHypercubeSEQ::CreateHypercubeComm(int dims) {
  std::vector<int> dims_array(dims, 2);
  std::vector<int> periods(dims, 1);
  MPI_Comm cart_comm;
  int result = MPI_Cart_create(MPI_COMM_WORLD, dims, dims_array.data(), periods.data(), 0, &cart_comm);
  if (result != MPI_SUCCESS) {
    return MPI_COMM_NULL;
  }
  return cart_comm;
}

int KolotukhinAHypercubeSEQ::CalculateHypercubeDimension(int num_processes) {
  int dimension = 0;
  while (num_processes > 1) {
    num_processes >>= 1;
    dimension++;
  }
  return dimension;
}

void KolotukhinAHypercubeSEQ::PerformComputeLoad(int iterations) {
  volatile std::atomic<double> compute_load{0.0};

  for (int iter = 0; iter < iterations; iter++) {
    double val = static_cast<double>(iter);
    compute_load.store(compute_load.load() + std::sin(val * 0.0001) * std::cos(val * 0.0001));
    if (iter % 1000 == 0) {
      double temp = compute_load.load();
      compute_load.store(std::fmod(temp, 1000.0));
    }
  }
  double final_result = compute_load.load();
  (void)final_result;
}

KolotukhinAHypercubeSEQ::KolotukhinAHypercubeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = {};
}

bool KolotukhinAHypercubeSEQ::ValidationImpl() {
  return true;
}

bool KolotukhinAHypercubeSEQ::PreProcessingImpl() {
  GetOutput().data.clear();
  GetOutput() = {{}, -1, valid_};
  return true;
}

bool KolotukhinAHypercubeSEQ::RunImpl() {
  int world_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if ((world_size <= 0) || ((world_size & (world_size - 1)) != 0) || (GetInput().source < 0) ||
      (GetInput().source > world_size - 1)) {
    valid_ = false;
    GetOutput() = {std::vector<int>{}, -1, valid_};
  }

  if (valid_) {
    int world_rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const auto &input = GetInput();
    int source = input.source;
    int dest = input.dest;

    std::vector<int> data;
    std::uint64_t data_size = 0;

    if (world_rank == source) {
      data.reserve(input.data_size);
      for (std::size_t i = 0; i < input.data_size; i++) {
        data.push_back(static_cast<int>(i) * 2 + 1);
      }
      data_size = data.size();
    }

    int dimensions = CalculateHypercubeDimension(world_size);
    hypercube_comm_ = CreateHypercubeComm(dimensions);
    int cube_rank = -1;
    MPI_Comm_rank(hypercube_comm_, &cube_rank);

    int current = source;

    for (int dim = 0; dim < dimensions; dim++) {
      if ((source ^ dest) & (1 << dim)) {
        int next = current ^ (1 << dim);
        if (cube_rank == current) {
          PerformComputeLoad(10000);
          MPI_Send(&data_size, 1, MPI_UINT64_T, next, 0, hypercube_comm_);
          MPI_Send(data.data(), static_cast<int>(data_size), MPI_INT, next, 1, hypercube_comm_);
        } else if (cube_rank == next) {
          MPI_Recv(&data_size, 1, MPI_UINT64_T, current, 0, hypercube_comm_, MPI_STATUS_IGNORE);
          data.resize(data_size);
          MPI_Recv(data.data(), static_cast<int>(data_size), MPI_INT, current, 1, hypercube_comm_, MPI_STATUS_IGNORE);
          PerformComputeLoad(10000);
        }
        current = next;
        if (current == dest) {
          break;
        }
      }
    }

    if (world_rank == dest) {
      GetOutput().data = data;
      GetOutput().process_id = dest;
    } else if (world_rank == source) {
      GetOutput().data = data;
      GetOutput().process_id = source;
    } else {
      GetOutput().data = std::vector<int>{};
      GetOutput().process_id = world_rank;
    }

    MPI_Comm_free(&hypercube_comm_);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  return true;
}

bool KolotukhinAHypercubeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
