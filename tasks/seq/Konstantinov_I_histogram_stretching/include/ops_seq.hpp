#pragma once
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace konstantinov_i_linear_histogram_stretch_seq {
std::vector<int> GetRandomImage(int sz);
void LinearHistogramStretch(const std::vector<int>& input, std::vector<int>& output);

class LinearHistogramStretchSeq : public ppc::core::Task {
 public:
  explicit LinearHistogramStretchSeq(std::shared_ptr<ppc::core::TaskData> task_data) : Task(std::move(task_data)) {}
  bool PreProcessingImpl() override;
  bool ValidationImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

 private:
  std::vector<int> I;
  std::vector<int> image_input;
  std::vector<int> image_output;
};
}  // namespace konstantinov_i_linear_histogram_stretch_seq