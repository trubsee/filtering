#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <random>

#include "common/assert.hpp"
#include "common/constants.hpp"

namespace Common {

namespace {
// mersenne_twister - slower but more random
// using Engine = std::mt19937;

// subtract with carry - fast but not very random
// using Engine = std::subtract_with_carry_engine<uint32_t, 32, 10, 24>;

using Engine =
    std::linear_congruential_engine<uint32_t, 1664525, 1013904223, 4294967295>;
Engine gen(RANDOM_SEED);
std::normal_distribution<double> norm{0, 1};
}  // namespace

static double SampleNormal() { return norm(gen); }

template <typename MatrixType>
static auto SampleMvNormal(const MatrixType& tril) {
    ASSERT(tril.rows() == tril.cols());
    using VectorType = typename std::conditional<
        Eigen::internal::traits<MatrixType>::RowsAtCompileTime ==
            Eigen::Dynamic,
        Eigen::VectorXd,
        Eigen::Vector<double, MatrixType::RowsAtCompileTime>>::type;

    VectorType randomVariables(tril.rows());
    for (unsigned i = 0; i < tril.rows(); ++i)
        randomVariables(i) = SampleNormal();
    return VectorType{tril * randomVariables};
}

class DiscreteDistribution {
   public:
    DiscreteDistribution(const std::vector<double>& weights)
        : mDistribution{weights.begin(), weights.end()} {}

    int Sample() { return mDistribution(gen); }

   private:
    std::discrete_distribution<int> mDistribution;
};

}  // namespace Common
