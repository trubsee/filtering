#pragma once

#include <Eigen/Dense>

namespace StochasticModels {

class StochasticVolatility {
  // prototype a simple StochasticVol model initially
  using Matrix = Eigen::Matrix<double, 1, 1>;
  using Vector = Eigen::Vector<double, 1>;

 public:
  StochasticVolatility(const double coefficient) : mCoef{coefficient} {}

  Vector Mutate(const Vector&) const;

  double Probability(const Vector&, const Vector&) const;

 private:
  const double mCoef;
};

}  // namespace StochasticModels
