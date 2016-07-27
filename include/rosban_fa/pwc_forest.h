#pragma once

#include "rosban_fa/function_approximator.h"

#include "rosban_regression_forests/core/forest.h"

namespace rosban_fa
{

class PWCForest : public FunctionApproximator
{
public:
  typedef std::vector<std::unique_ptr<regression_forests::Forest>> Forests;

  PWCForest(std::unique_ptr<Forests> forests,
            int max_action_tiles);

  virtual ~PWCForest();

  virtual int getOutputDim() const override;

  /// Predict the outputs independently using internal structure
  virtual void predict(const Eigen::VectorXd & input,
                       Eigen::VectorXd & mean,
                       Eigen::MatrixXd & covar) const override;

  virtual void gradient(const Eigen::VectorXd & input,
                        Eigen::VectorXd & gradient) const override;

  virtual void getMaximum(const Eigen::MatrixXd & limits,
                          Eigen::VectorXd & input,
                          double & output) const override;

private:
  std::unique_ptr<Forests> forests;
  int max_action_tiles;
};

}
