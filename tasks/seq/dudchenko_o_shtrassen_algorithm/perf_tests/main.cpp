#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "core/task/include/task.hpp"
#include "seq/dudchenko_o_shtrassen_algorithm/include/ops_seq.hpp"

namespace {
struct Value {
  double min_value;
  double max_value;
};

std::vector<double> GenerateRandomSquareMatrix(int n, Value value) {
  std::vector<double> matrix(n * n);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(value.min_value, value.max_value);

  for (int i = 0; i < n * n; ++i) {
    matrix[i] = dis(gen);
  }
  return matrix;
}
}  // namespace

TEST(dudchenko_o_shtrassen_algorithm_seq, test_pipeline_run) {
  const size_t n = 128;

  std::vector<double> a = GenerateRandomSquareMatrix(n, {.min_value = -50, .max_value = 50});
  std::vector<double> b = GenerateRandomSquareMatrix(n, {.min_value = -50, .max_value = 50});

  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(a.data()));
  task_data_seq->inputs_count.emplace_back(a.size());
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(b.data()));
  task_data_seq->inputs_count.emplace_back(b.size());

  std::vector<double> out(n * n, 0.0);
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  auto test_task_sequential =
      std::make_shared<dudchenko_o_shtrassen_algorithm_seq::StrassenAlgoriphmSequential>(task_data_seq);
  ASSERT_TRUE(test_task_sequential->ValidationImpl());
  ASSERT_TRUE(test_task_sequential->PreProcessingImpl());
  ASSERT_TRUE(test_task_sequential->RunImpl());
  ASSERT_TRUE(test_task_sequential->PostProcessingImpl());

  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 2;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perf_results = std::make_shared<ppc::core::PerfResults>();
  auto perf_analyzer = std::make_shared<ppc::core::Perf>(test_task_sequential);
  perf_analyzer->PipelineRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);
}

TEST(dudchenko_o_shtrassen_algorithm_seq, test_task_run) {
  const size_t n = 128;

  std::vector<double> a = GenerateRandomSquareMatrix(n, {.min_value = -50, .max_value = 50});
  std::vector<double> b = GenerateRandomSquareMatrix(n, {.min_value = -50, .max_value = 50});

  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(a.data()));
  task_data_seq->inputs_count.emplace_back(a.size());
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(b.data()));
  task_data_seq->inputs_count.emplace_back(b.size());

  std::vector<double> out(n * n, 0.0);
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  auto test_task_sequential =
      std::make_shared<dudchenko_o_shtrassen_algorithm_seq::StrassenAlgoriphmSequential>(task_data_seq);

  ASSERT_TRUE(test_task_sequential->ValidationImpl());
  ASSERT_TRUE(test_task_sequential->PreProcessingImpl());
  ASSERT_TRUE(test_task_sequential->RunImpl());
  ASSERT_TRUE(test_task_sequential->PostProcessingImpl());

  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 2;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perf_results = std::make_shared<ppc::core::PerfResults>();
  auto perf_analyzer = std::make_shared<ppc::core::Perf>(test_task_sequential);
  perf_analyzer->TaskRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);
}