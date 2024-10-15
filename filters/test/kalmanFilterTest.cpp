#include <cmath>
#include <numeric>
#include <random>

#include "libalglib/statistics.h"
#include "gtest/gtest.h"

#include "filters/kalmanFilter.ipp"

namespace Filters::Test {

TEST(KalmanFilterTest, CheckZeroDrift)
{
    BasicKF<2, 2> kf {
        Eigen::Vector2d{0, 10},
        Eigen::Matrix2d{{1, 0}, {0, 5}},
        Eigen::Matrix2d{{1, 0}, {0, 1}},
        Eigen::Matrix2d{{1, 0}, {0, 1}},
        Eigen::Matrix2d{{0, 0}, {0, 0}},
        Eigen::Matrix2d{{0.5, 0}, {0, 2}}
    };

    for (unsigned i = 0; i < 1000 ; ++i)
    {
        kf.Update(Eigen::Vector2d{1, 12});
    }
    EXPECT_NEAR(kf.Estimate()(0), 1, 0.01);
    EXPECT_NEAR(kf.Estimate()(1), 12, 0.01);
}

TEST(KalmanFilterTest, ObsMoreThanHidden)
{
    BasicKF<2, 3> kf {
        Eigen::Vector2d{0, 0},
        Eigen::Matrix2d{{1, 0}, {0, 1}},
        Eigen::Matrix2d{{1, 0}, {0, 1}},
        Eigen::MatrixXd{{1, 0}, {1, 0}, {0, 1}},
        Eigen::Matrix2d{{0.1, 0}, {0, 0.1}},
        Eigen::Matrix3d{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}
    };

    for (unsigned i = 0; i < 1000; ++i)
    {
        kf.Update(Eigen::Vector3d{10.1, 9.9, 5.});
    }
    EXPECT_NEAR(kf.Estimate()(0), 10, 0.01);
    EXPECT_NEAR(kf.Estimate()(1), 5, 0.01);
}

TEST(KalmanFilterTest, HypothesisTest)
{
    std::random_device rd{};
    std::mt19937 gen{rd()};

    const double initial{0.};
    const double drift{0.5};
    std::normal_distribution<double> driftG{0, std::sqrt(drift)};
    const double noise{0.1};
    std::normal_distribution<double> noiseG{0, std::sqrt(noise)};

    BasicKF<1, 1> kf {
        Eigen::MatrixXd{{initial}},
        Eigen::MatrixXd{{1}},
        Eigen::MatrixXd{{1}},
        Eigen::MatrixXd{{1}},
        Eigen::MatrixXd{{drift}},
        Eigen::MatrixXd{{noise}}
    };

    const unsigned ITER{10000};
    std::vector<double> innovations(ITER);
    std::vector<double> normInnovations(ITER);
    auto hiddenState = initial + driftG(gen);

    for (unsigned i = 0; i < ITER; ++i)
    {
        // generate fake obs
        auto obs = hiddenState + noiseG(gen);
        kf.Update(Eigen::VectorXd{{obs}});
       
        // store data
        auto [innovation, innVar] = kf.GetLastInnovation();
        innovations.emplace_back(innovation(0, 0));
        normInnovations.emplace_back(std::pow(innovation(0, 0), 2) / innVar(0, 0));
        hiddenState += driftG(gen);
    }
    
    // Chi-squared Test
    const auto innovationSum = std::accumulate(normInnovations.begin(), normInnovations.end(), 0.);
    EXPECT_NEAR(innovationSum / ITER, 1.1, 0.1);

    // Auto-Correlation Test
    alglib::real_1d_array v1;
    v1.setcontent(innovations.size()-1, &(innovations[0]));
    alglib::real_1d_array v2;
    v2.setcontent(innovations.size()-1, &(innovations[1]));
    EXPECT_NEAR(alglib::pearsoncorr2(v1, v2), 0., 0.1);
}

}
