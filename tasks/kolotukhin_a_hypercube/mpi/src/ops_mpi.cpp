#include "kolotukhin_a_hypercube/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "kolotukhin_a_hypercube/common/include/common.hpp"

namespace kolotukhin_a_hypercube {

KolotukhinAHypercubeMPI::KolotukhinAHypercubeMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput().data = std::vector<int>{};
  GetOutput().process_id = -1;
  GetOutput().exec = true;
}

int KolotukhinAHypercubeMPI::GetNeighbor(int rank, int dim) {
  int neighbor = rank ^ (1 << dim);
  return neighbor;
}

int KolotukhinAHypercubeMPI::CalculateHypercubeDimension(int num_processes) {
  int dimension = 0;
  while (num_processes > 1) {
    num_processes >>= 1;
    dimension++;
  }
  return dimension;
}

void KolotukhinAHypercubeMPI::PerformComputeLoad(int iterations) {
  std::atomic<double> compute_load{0.0};
  double temp = 0.0;
  double val = 0.0;
  for (int iter = 0; iter < iterations; iter++) {
    val = static_cast<double>(iter);
    compute_load.store(compute_load.load() + (std::sin(val * 0.0001) * std::cos(val * 0.0001)));
    if (iter % 1000 == 0) {
      temp = compute_load.load();
      compute_load.store(std::fmod(temp, 1000.0));
    }
  }
  [[maybe_unused]] double final_result = compute_load.load();
}

void KolotukhinAHypercubeMPI::SendData(std::vector<int> &data, int next_neighbor) {
  std::uint64_t data_size = data.size();
  MPI_Send(&data_size, 1, MPI_UINT64_T, next_neighbor, 0, MPI_COMM_WORLD);
  if (data_size > 0) {
    MPI_Send(data.data(), static_cast<int>(data_size), MPI_INT, next_neighbor, 1, MPI_COMM_WORLD);
  }
}

void KolotukhinAHypercubeMPI::RecvData(std::vector<int> &data, int prev_neighbor) {
  std::uint64_t data_size = 0;
  MPI_Recv(&data_size, 1, MPI_UINT64_T, prev_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  data.resize(data_size);
  if (data_size > 0) {
    MPI_Recv(data.data(), static_cast<int>(data_size), MPI_INT, prev_neighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
}

void KolotukhinAHypercubeMPI::CalcPositions(int my_rank, std::vector<int> &path, int &my_pos, int &next, int &prev) {
  for (std::size_t i = 0; i < path.size(); i++) {
    if (my_rank == path[i]) {
      my_pos = static_cast<int>(i);
      if (i > 0) {
        prev = path[i - 1];
      }
      if (i < path.size() - 1) {
        next = path[i + 1];
      }
      break;
    }
  }
}

std::vector<int> KolotukhinAHypercubeMPI::CalcPath(int source, int dest, int dimensions) {
  std::vector<int> path;
  int current = source;
  path.push_back(current);
  int xor_val = source ^ dest;
  for (int dim = 0; dim < dimensions; dim++) {
    int mask = 1 << dim;
    if ((xor_val & mask) != 0) {
      current = current ^ mask;
      path.push_back(current);
      if (current == dest) {
        break;
      }
    }
  }
  return path;
}

bool KolotukhinAHypercubeMPI::ValidationImpl() {
  int world_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if ((GetInput().source < 0) || (GetInput().source > world_size - 1) ||
      ((GetInput().dest < 0) && (GetInput().dest != -2)) || (GetInput().dest > world_size - 1) || (world_size <= 0) ||
      ((world_size & (world_size - 1)) != 0)) {
    exec_ = false;
  }
  return true;
}

bool KolotukhinAHypercubeMPI::PreProcessingImpl() {
  GetOutput().data.clear();
  return true;
}

bool KolotukhinAHypercubeMPI::RunImpl() {
  int rank = -1;
  int world_size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (!exec_) {
    GetOutput().data.clear();
    GetOutput().process_id = -1;
    GetOutput().exec = exec_;
    MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }

  const auto &input = GetInput();
  int source = input.source;
  int dest = input.dest;
  if (dest == -2) {
    dest = world_size - 1;
  }
  std::vector<int> data{};
  std::uint64_t data_size = 0;

  int dimensions = 0;
  dimensions = CalculateHypercubeDimension(world_size);

  if (rank == source) {
    data_size = static_cast<std::uint64_t>(GetInput().data_size);
    data.resize(data_size);
    for (size_t i = 0; i < data_size; i++) {
      data[i] = (static_cast<int>(i) * 2) + 1;
    }
  }

  if (source == dest) {
    GetOutput().data = data;
    GetOutput().process_id = rank;
    GetOutput().exec = exec_;
    return true;
  }

  std::vector<int> path = CalcPath(source, dest, dimensions);
  if (path.back() != dest) {
    GetOutput().data.clear();
    GetOutput().process_id = -1;
    GetOutput().exec = exec_;
    MPI_Barrier(MPI_COMM_WORLD);
    return false;
  }
  int my_position = -1;
  int prev_neighbor = -1;
  int next_neighbor = -1;
  CalcPositions(rank, path, my_position, next_neighbor, prev_neighbor);

  if (rank == source) {
    PerformComputeLoad(150000);
    SendData(data, next_neighbor);
  } else if (rank == dest) {
    RecvData(data, prev_neighbor);
    PerformComputeLoad(150000);
  } else {
    RecvData(data, prev_neighbor);
    PerformComputeLoad(150000);
    SendData(data, next_neighbor);
    data.clear();
  }

  if (rank == dest || rank == source) {
    GetOutput().data = data;
  } else {
    GetOutput().data = std::vector<int>();
  }
  GetOutput().process_id = rank;
  GetOutput().exec = exec_;
  MPI_Barrier(MPI_COMM_WORLD);
  return true;
}

bool KolotukhinAHypercubeMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kolotukhin_a_hypercube
