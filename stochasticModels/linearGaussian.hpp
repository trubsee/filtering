#pragma once

#include <Eigen/Cholesky>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>

#include "common/assert.hpp"
#include "common/constants.hpp"
#include "common/randomSample.hpp"
#include "stochasticModels/linearGaussianTraits.hpp"

namespace StochasticModels {

namespace {

template <typename Traits>
class LinearGaussianImpl {
    using CoefMatrix = typename Traits::CoefMatrix;
    using NoiseMatrix = typename Traits::NoiseMatrix;
    using InputVector = typename Traits::InputVector;
    using OutputVector = typename Traits::OutputVector;

   public:
    LinearGaussianImpl(const CoefMatrix& coefficients,
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
    CoefMatrix mCoef;
    NoiseMatrix mTril;
    NoiseMatrix mInverseNoise;
    double mMaxProb;
    
    unsigned mInputs;
    unsigned mOutputs;
};

}  // namespace

struct LinearGaussian {
    template <int Input, int Output>
    using Static = LinearGaussianImpl<StaticLGTraits<Input, Output>>;
    using Dynamic = LinearGaussianImpl<DynamicLGTraits>;
};

}  // namespace StochasticModels
