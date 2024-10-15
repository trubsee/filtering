#pragma once

#include <cmath>
#include <iostream>

#include "common/constants.hpp"
#include "stochasticModels/randomWalk.hpp"

namespace StochasticModels {

template <int num>
RandomWalk<num>::RandomWalk(const Matrix& covarianceMatrix)
:
    mRandomDevice{},
    mGen{mRandomDevice()}
{
    mTril = covarianceMatrix.llt().matrixL();
    mInverseNoise = covarianceMatrix.inverse();
    mMaxProb = 1 / std::sqrt(std::pow(2 * PI, num) * covarianceMatrix.determinant());
}

template <int num>
typename RandomWalk<num>::Vector RandomWalk<num>::Mutate(const Vector& input) const 
{
    Vector randomVariables; 
    for (unsigned i = 0; i < num; ++i)
        randomVariables(i) = mNorm(mGen);
    return input + mTril * randomVariables;
}

template <int num>
double RandomWalk<num>::Probability(const Vector& m1, const Vector& m2) const
{
    const auto relative = (m1 - m2).transpose() * mInverseNoise * (m1 - m2);
    return mMaxProb * std::exp(-relative(0));
}

}
