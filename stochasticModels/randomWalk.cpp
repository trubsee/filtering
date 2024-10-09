#include <cmath>
#include <iostream>

#include <Eigen/Cholesky>

#include "common/assert.hpp"
#include "common/constants.hpp"
#include "stochasticModels/randomWalk.hpp"

namespace StochasticModels {

RandomWalk::RandomWalk(const Eigen::MatrixXd& covarianceMatrix)
:
    mNumVariables{covarianceMatrix.rows()},
    mRandomDevice{},
    mGen{mRandomDevice()}
{
    ASSERT(covarianceMatrix.rows() == covarianceMatrix.cols());
    mTril = covarianceMatrix.llt().matrixL();
}

Eigen::MatrixXd RandomWalk::Mutate(const Eigen::MatrixXd& input) const 
{
    ASSERT(input.cols() == 1 && input.rows() == mNumVariables);
    Eigen::VectorXd randomVariables(mNumVariables); 
    for (unsigned i = 0; i < mNumVariables; ++i)
        randomVariables(i) = mNorm(mGen);
    return input + mTril * randomVariables;
}

double RandomWalk::Probability(const Eigen::MatrixXd& m1, const Eigen::MatrixXd& m2) const
{
    const double max = 1 / std::sqrt(std::pow(2 * PI, mNumVariables)) / mTril.determinant();
    const auto noise = GetNoiseMatrix();
    const auto relative = (m1 - m2).transpose() * noise.inverse() * (m1 - m2);
    return max * std::exp(-relative(0, 0));
}

}
