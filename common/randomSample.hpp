#pragma once

#include <iostream>
#include <random>

#include <Eigen/Dense>

#include "common/assert.hpp"
#include "common/constants.hpp"

namespace Common {

namespace {

    // std::random_device mRandomDevice;
    std::mt19937 mGen{ RANDOM_SEED };
    std::normal_distribution<double> mNorm{0, 1};

    Eigen::VectorXd SampleMvNormalGeneric(const Eigen::MatrixXd& tril, std::mt19937& gen, std::normal_distribution<double>& norm)
    {
        ASSERT(tril.rows() == tril.cols());
        Eigen::VectorXd randomVariables(tril.rows());
        for (unsigned i = 0; i < tril.rows(); ++i)
            randomVariables(i) = norm(gen);
        return tril * randomVariables;
    }
}


static double SampleNormal()
{
    return mNorm(mGen);
}


static Eigen::VectorXd SampleMvNormal(const Eigen::MatrixXd& tril)
{
    return SampleMvNormalGeneric(tril, mGen, mNorm);
}


class MvNormalSampler
{
public:
    Eigen::VectorXd SampleMvNormal(const Eigen::MatrixXd& tril)
    {
        return SampleMvNormalGeneric(tril, mGen, mNorm);
    }

private:
    std::mt19937 mGen{ RANDOM_SEED };
    std::normal_distribution<double> mNorm{0, 1};
};


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
