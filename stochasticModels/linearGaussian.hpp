#pragma once

#include <Eigen/Cholesky>
#include <Eigen/Dense>
#include <cmath>

#include "common/assert.hpp"
#include "common/constants.hpp"
#include "common/randomSample.hpp"
#include "stochasticModels/linearGaussianTraits.hpp"

namespace StochasticModels {

template <int Input, int Output>
class LinearGaussian {
    using Traits = LinearGaussianTraits<Input, Output>;
    using CoefMatrix = typename Traits::CoefMatrix;
    using NoiseMatrix = typename Traits::NoiseMatrix;
    using InputVector = typename Traits::InputVector;
    using OutputVector = typename Traits::OutputVector;

   public:
    LinearGaussian(const CoefMatrix& coefficients,
                   const NoiseMatrix& covarianceMatrix)
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

    OutputVector Mutate(const InputVector& input) const {
        ASSERT(input.size() == mInputs);
        return mCoef * input + Common::SampleMvNormal(mTril);
    }

    double Probability(const Eigen::VectorXd& output,
                       const Eigen::VectorXd& input) const {
        ASSERT(output.size() == mOutputs);
        ASSERT(input.size() == mInputs);
        const auto relative = (output - mCoef * input).transpose() *
                              mInverseNoise * (output - mCoef * input);
        ASSERT(relative.size() == 1);
        return mMaxProb * std::exp(-relative(0));
    }

    CoefMatrix GetCoefMatrix() const { return mCoef; }

    NoiseMatrix GetNoiseMatrix() const { return mTril * mTril.transpose(); }

    unsigned GetNumInputs() const { return mInputs; }

    unsigned GetNumOutputs() const { return mOutputs; }

   private:
    const unsigned mInputs;
    const unsigned mOutputs;

    CoefMatrix mCoef;
    NoiseMatrix mTril;
    NoiseMatrix mInverseNoise;
    double mMaxProb;
};

}  // namespace StochasticModels
