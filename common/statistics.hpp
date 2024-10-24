#pragma once

#include <Eigen/Dense>

#include "common/assert.hpp"

namespace Common {

static Eigen::MatrixXd CalculateCovariance(const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y)
{
    ASSERT(X.rows() == Y.rows());

    const auto n = X.rows();
    const auto meanX = X.colwise().mean();
    const auto meanY = Y.colwise().mean();

    const auto centredX = X.rowwise() - meanX;
    const auto centredY = Y.rowwise() - meanY;

    return (centredX.transpose() * centredY) / (n - 1);
}

}
