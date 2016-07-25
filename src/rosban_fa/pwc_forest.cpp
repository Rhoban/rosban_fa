#include "rosban_fa/pwc_forest.h"

#include "rosban_regression_forests/algorithms/extra_trees.h"
#include "rosban_regression_forests/approximations/approximation_type.h"
#include "rosban_regression_forests/approximations/pwc_approximation.h"

using regression_forests::ApproximationType;
using regression_forests::PWCApproximation;
using regression_forests::ExtraTrees;
using regression_forests::TrainingSet;
using regression_forests::Forest;
using regression_forests::Tree;
using regression_forests::Node;

namespace rosban_fa
{

PWCForest::PWCForest(std::unique_ptr<Forests> forests_,
                     int max_action_tiles_)
  : forests(std::move(forests_)), max_action_tiles(max_action_tiles_)
{}

PWCForest::~PWCForest() {}

int PWCForest::getOutputDim() const
{
  if (!forests) return 0;
  return forests->size();
}

void PWCForest::predict(const Eigen::VectorXd & input,
                        Eigen::VectorXd & mean,
                        Eigen::MatrixXd & covar) const
{
  int O = getOutputDim();
  mean = Eigen::VectorXd::Zero(O);
  covar = Eigen::MatrixXd::Zero(O,O);
  Eigen::VectorXd vars = Eigen::VectorXd::Zero(O);
  for (int output_dim = 0; output_dim < O; output_dim++)
  {
    mean(output_dim) = (*forests)[output_dim]->getValue(input);
    covar(output_dim, output_dim) = (*forests)[output_dim]->getVar(input);
  }
}

void PWCForest::gradient(const Eigen::VectorXd & input,
                         Eigen::VectorXd & gradient) const
{
  (void) input;(void) gradient;
  throw std::runtime_error("PWCForest::gradients: not implemented");
}

void PWCForest::getMaximum(const Eigen::MatrixXd & limits,
                           Eigen::VectorXd & input,
                           double & output) const
{
  check1DOutput("getMaximum");
  std::unique_ptr<regression_forests::Tree> sub_tree;
  sub_tree = (*forests)[0]->unifiedProjectedTree(limits, max_action_tiles);

  std::pair<double, Eigen::VectorXd> max_pair;
  max_pair = sub_tree->getMaxPair(limits);

  input = max_pair.second;
  output = max_pair.first;
}

}
