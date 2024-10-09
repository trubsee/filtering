#include "libalglib/statistics.h"
#include "gtest/gtest.h"

#include "common/constants.hpp"
#include "stochasticModels/randomWalk.hpp"

namespace StochasticModels::Test {

TEST(RandomWalkTest, CheckParam)
{
    const Eigen::MatrixXd cov {{1, 0.5}, {0.5, 2}};
    EXPECT_EQ(cov.rows(), cov.cols());

    RandomWalk rw{cov};
  
    EXPECT_EQ(rw.GetCoeffMatrix(), Eigen::MatrixXd::Identity(2, 2));
    EXPECT_EQ(rw.GetNoiseMatrix(), cov);
}

TEST(RandomWalkTest, CheckMutate)
{
    const Eigen::MatrixXd initial {{1}, {12}};
    const Eigen::MatrixXd cov {{1, 0.5}, {0.5, 2}};
    EXPECT_EQ(cov.rows(), cov.cols());
    RandomWalk rw{cov};

    const unsigned ITER{100000};
    std::vector<double> mutations (ITER * 2);
    for (unsigned i = 0; i < ITER; ++i)
    {
        const auto mutated = rw.Mutate(initial);
        for (unsigned j = 0; j < mutated.size(); ++j)
            mutations[i*2+j] = *(mutated.data()+j);
    }
    alglib::real_2d_array vals;
    alglib::real_2d_array dataCov;
    vals.setcontent(ITER, cov.rows(), &(mutations[0]));
    alglib::covm2(vals, vals, dataCov);
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
    const Eigen::MatrixXd cov {{1, 0.5}, {0.5, 2}};
    EXPECT_EQ(cov.rows(), cov.cols());
    RandomWalk rw{cov};
    
    const Eigen::MatrixXd m1 {{4}, {6}};
    auto prob = rw.Probability(m1, m1);
 
    auto max = 1 / std::sqrt(std::pow(2 * PI, 2) * cov.determinant());
    EXPECT_EQ(prob, max);

    const Eigen::MatrixXd m2 {{3}, {6}};
    prob = rw.Probability(m1, m2);
    auto relative = (m1 - m2).transpose() * cov.inverse() * (m1 - m2);
    EXPECT_EQ(max * std::exp(-relative(0, 0)), prob);
}

}
