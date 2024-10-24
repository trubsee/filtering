#include "gtest/gtest.h"

#include "common/constants.hpp"
#include "common/statistics.hpp"
#include "stochasticModels/linearFactoryMethods.hpp"

namespace StochasticModels::Test {

TEST(RandomWalkTest, CheckParam)
{
    const Eigen::Matrix2d cov {{1, 0.5}, {0.5, 2}};
    EXPECT_EQ(cov.rows(), cov.cols());

    auto rw {CreateRandomWalk(cov)};
  
    EXPECT_EQ(rw.GetCoefMatrix(), Eigen::MatrixXd::Identity(2, 2));
    EXPECT_EQ(rw.GetNoiseMatrix(), cov);
}

TEST(RandomWalkTest, CheckMutate)
{
    const Eigen::Vector2d initial {1, 12};
    const Eigen::Matrix2d cov {{1, 0.5}, {0.5, 2}};
    EXPECT_EQ(cov.rows(), cov.cols());
    auto rw{CreateRandomWalk(cov)};

    const unsigned ITER{100000};
    Eigen::MatrixXd mutations(ITER, 2);
    for (unsigned i = 0; i < ITER; ++i)
    {
        mutations.block(i, 0, 1, 2) = rw.Mutate(initial).transpose();
    }
    const auto dataCov = Common::CalculateCovariance(mutations, mutations);
    
    EXPECT_EQ(dataCov.rows(), cov.rows());
    EXPECT_EQ(dataCov.cols(), cov.cols());

    for (unsigned i = 0; i < cov.rows(); ++i)
    {
        for (unsigned j = 0; j < cov.cols(); ++j)
            EXPECT_NEAR(dataCov(i, j), cov(i, j), 0.05);
    }
}

TEST(RandomWalkTest, CheckProbability)
{
    const Eigen::Matrix2d cov {{1, 0.5}, {0.5, 2}};
    EXPECT_EQ(cov.rows(), cov.cols());
    auto rw {CreateRandomWalk(cov)};
    
    const Eigen::Vector2d m1 {4, 6};
    auto prob = rw.Probability(m1, m1);
 
    auto max = 1 / std::sqrt(std::pow(2 * PI, 2) * cov.determinant());
    EXPECT_EQ(prob, max);

    const Eigen::Vector2d m2 {3, 6};
    prob = rw.Probability(m1, m2);
    auto relative = (m1 - m2).transpose() * cov.inverse() * (m1 - m2);
    EXPECT_EQ(max * std::exp(-relative(0)), prob);
}

}
