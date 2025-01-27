#pragma once

#include <Eigen/Cholesky>
#include <Eigen/Dense>

#include "common/assert.hpp"
#include "stochasticModels/linearGaussian.hpp"

namespace StochasticModels {

template <int NumVar>
static LinearGaussian<NumVar, NumVar> CreateRandomWalk(
    const Eigen::MatrixXd& covarianceMatrix) {
    ASSERT(covarianceMatrix.rows() == covarianceMatrix.cols());
    const auto size = covarianceMatrix.rows();
    return LinearGaussian<NumVar, NumVar>{Eigen::MatrixXd::Identity(size, size),
                                          covarianceMatrix};
}

}  // namespace StochasticModels
