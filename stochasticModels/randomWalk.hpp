#pragma once

#include <random>

#include <Eigen/Dense>

namespace StochasticModels {

class RandomWalk
{
public:
    RandomWalk(const Eigen::MatrixXd& covarianceMatrix);

    Eigen::MatrixXd Mutate(const Eigen::MatrixXd& input) const;

    double Probability(const Eigen::MatrixXd& m1, const Eigen::MatrixXd& m2) const;

    // Linear Operators
    Eigen::MatrixXd GetCoeffMatrix() const { return Eigen::MatrixXd::Identity(mNumVariables, mNumVariables); }

    Eigen::MatrixXd GetNoiseMatrix() const { return mTril * mTril.transpose(); }

private:
    const Eigen::Index mNumVariables;
    Eigen::MatrixXd mTril;

    std::random_device mRandomDevice;
    mutable std::mt19937 mGen;
    mutable std::normal_distribution<double> mNorm{0, 1};
};

}
