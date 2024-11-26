#include "stochasticModels/linearGaussian.hpp"

#include <cmath>
#include <iostream>

#include "common/assert.hpp"
#include "common/constants.hpp"
#include "common/randomSample.hpp"

namespace StochasticModels {

LinearGaussian::LinearGaussian(const Eigen::MatrixXd& coefficients,
                               const Eigen::MatrixXd& covarianceMatrix)
    : mOutputs{(unsigned)coefficients.rows()},
      mInputs{(unsigned)coefficients.cols()},
      mCoef{coefficients} {
    ASSERT(covarianceMatrix.rows() == covarianceMatrix.cols());
    ASSERT(covarianceMatrix.rows() == mOutputs);
    mTril = covarianceMatrix.llt().matrixL();
    mInverseNoise = covarianceMatrix.inverse();
    mMaxProb = 1 / std::sqrt(std::pow(2 * PI, mOutputs) *
                             covarianceMatrix.determinant());
}

Eigen::VectorXd LinearGaussian::Mutate(const Eigen::VectorXd& input) const {
    ASSERT(input.size() == mInputs);
    return mCoef * input + Common::SampleMvNormal(mTril);
}

double LinearGaussian::Probability(const Eigen::VectorXd& output,
                                   const Eigen::VectorXd& input) const {
    ASSERT(output.size() == mOutputs);
    ASSERT(input.size() == mInputs);
    const auto relative = (output - mCoef * input).transpose() * mInverseNoise *
                          (output - mCoef * input);
    ASSERT(relative.size() == 1);
    return mMaxProb * std::exp(-relative(0));
}

}  // namespace StochasticModels
