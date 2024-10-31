#pragma once

#include <iostream>
#include <random>

#include <Eigen/Dense>

#include "common/assert.hpp"

namespace Common {

namespace {

    // std::random_device mRandomDevice;
    std::mt19937 mGen{ 123 };
    std::normal_distribution<double> mNorm{0, 1};
}

static double SampleNormal()
{
    return mNorm(mGen);
}

static Eigen::VectorXd SampleMvNormal(const Eigen::MatrixXd& tril)
{
    ASSERT(tril.rows() == tril.cols());
    Eigen::VectorXd randomVariables(tril.rows());
    for (unsigned i = 0; i < tril.rows(); ++i)
        randomVariables(i) = SampleNormal();
    return tril * randomVariables;
}

class DiscreteDistribution
{
public:
    DiscreteDistribution(const std::vector<double>& weights)
    :
        mDistribution{weights.begin(), weights.end()}
    {
    }

    int Sample()
    {
        return mDistribution(mGen);
    }

private:
    std::discrete_distribution<int> mDistribution;
};

}
