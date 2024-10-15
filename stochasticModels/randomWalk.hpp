#pragma once

#include <random>

#include <Eigen/Cholesky>
#include <Eigen/Dense>

namespace StochasticModels {

template <int num> 
class RandomWalk
{
public:
    using Matrix = Eigen::Matrix<double, num, num>;
    using Vector = Eigen::Vector<double, num>;

    RandomWalk(const Matrix& covarianceMatrix);

    Vector Mutate(const Vector& input) const;

    double Probability(const Vector& m1, const Vector& m2) const;

    // Linear Operators
    Matrix GetCoeffMatrix() const { return Matrix::Identity(); }

    Matrix GetNoiseMatrix() const { return mTril * mTril.transpose(); }

private:
    Matrix mTril;
    Matrix mInverseNoise;
    double mMaxProb;

    std::random_device mRandomDevice;
    mutable std::mt19937 mGen;
    mutable std::normal_distribution<double> mNorm{0, 1};
};

}
