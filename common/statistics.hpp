#pragma once

#include <Eigen/Dense>

#include "common/assert.hpp"

namespace Common {

static Eigen::MatrixXd CalculateCovariance(const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y)
{
    ASSERT(X.rows() == Y.rows());
    const auto n = X.rows();
    const auto centredX = X.rowwise() - X.colwise().mean();
    const auto centredY = Y.rowwise() - Y.colwise().mean();

    return (centredX.transpose() * centredY) / (n - 1);
}

}
