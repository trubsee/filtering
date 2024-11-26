#pragma once

#include <Eigen/Cholesky>
#include <Eigen/Dense>

namespace StochasticModels {

class LinearGaussian {
 public:
  LinearGaussian(const Eigen::MatrixXd&, const Eigen::MatrixXd&);

  Eigen::VectorXd Mutate(const Eigen::VectorXd&) const;

  double Probability(const Eigen::VectorXd&, const Eigen::VectorXd&) const;

  Eigen::MatrixXd GetCoefMatrix() const { return mCoef; }

  Eigen::MatrixXd GetNoiseMatrix() const { return mTril * mTril.transpose(); }

  unsigned GetNumInputs() const { return mInputs; }

  unsigned GetNumOutputs() const { return mOutputs; }

 private:
  const unsigned mInputs;
  const unsigned mOutputs;

  Eigen::MatrixXd mCoef;
  Eigen::MatrixXd mTril;
  Eigen::MatrixXd mInverseNoise;
  double mMaxProb;
};

}  // namespace StochasticModels
