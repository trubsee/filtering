#pragma once

#include <Eigen/Dense>

#include "stochasticModels/linearGaussian.hpp"

namespace Filters {

class KalmanFilter {
 public:
  KalmanFilter(const Eigen::VectorXd&, const Eigen::MatrixXd&,
               const StochasticModels::LinearGaussian&,
               const StochasticModels::LinearGaussian&);

  void Update(const Eigen::VectorXd&);

  const Eigen::VectorXd& Estimate() const { return x; }

  Eigen::VectorXd Predict() const { return F * x; }

  std::pair<const Eigen::VectorXd&, const Eigen::MatrixXd&> GetLastInnovation()
      const {
    return {y, S};
  }

 private:
  // model params
  Eigen::MatrixXd F;
  Eigen::MatrixXd H;
  Eigen::MatrixXd Q;
  Eigen::MatrixXd R;

  // dimensions
  const unsigned mHidden;
  const unsigned mObserved;

  // current estimate
  Eigen::VectorXd x;
  Eigen::MatrixXd P;
  // innovations
  Eigen::VectorXd y;
  Eigen::MatrixXd S;
};

}  // namespace Filters
