#pragma once

#include <Eigen/Cholesky>
#include <Eigen/Dense>

#include "common/assert.hpp"
#include "stochasticModels/linearGaussian.hpp"

namespace StochasticModels {

static LinearGaussian CreateRandomWalk(
    const Eigen::MatrixXd& covarianceMatrix) {
    ASSERT(covarianceMatrix.rows() == covarianceMatrix.cols());
    const auto size = covarianceMatrix.rows();
    return LinearGaussian{Eigen::MatrixXd::Identity(size, size),
                          covarianceMatrix};
}

}  // namespace StochasticModels
