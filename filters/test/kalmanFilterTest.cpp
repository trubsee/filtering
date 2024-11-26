#include <cmath>
#include <numeric>
#include <random>

#include "common/randomSample.hpp"
#include "filters/kalmanFilter.hpp"
#include "gtest/gtest.h"
#include "libalglib/statistics.h"
#include "stochasticModels/linearFactoryMethods.hpp"

namespace Filters::Test {

TEST(KalmanFilterTest, CheckZeroDrift) {
    const auto stateModel{
        StochasticModels::CreateRandomWalk(Eigen::Matrix2d{{0, 0}, {0, 0}})};
    const auto obsModel{
        StochasticModels::CreateRandomWalk(Eigen::Matrix2d{{0.5, 0}, {0, 2}})};

    KalmanFilter kf{Eigen::Vector2d{0, 10}, Eigen::Matrix2d{{1, 0}, {0, 5}},
                    stateModel, obsModel};

    for (unsigned i = 0; i < 1000; ++i) {
        kf.Update(Eigen::Vector2d{1, 12});
    }
    EXPECT_NEAR(kf.Estimate()(0), 1, 0.01);
    EXPECT_NEAR(kf.Estimate()(1), 12, 0.01);
}

TEST(KalmanFilterTest, ObsMoreThanHidden) {
    const auto stateModel{StochasticModels::CreateRandomWalk(
        Eigen::Matrix2d{{0.1, 0}, {0, 0.1}})};
    const StochasticModels::LinearGaussian obsModel{
        Eigen::MatrixXd{{1, 0}, {1, 0}, {0, 1}},
        Eigen::Matrix3d{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};

    KalmanFilter kf{Eigen::Vector2d{0, 0}, Eigen::Matrix2d{{1, 0}, {0, 1}},
                    stateModel, obsModel};

    for (unsigned i = 0; i < 1000; ++i) {
        kf.Update(Eigen::Vector3d{10.1, 9.9, 5.});
    }
    EXPECT_NEAR(kf.Estimate()(0), 10, 0.01);
    EXPECT_NEAR(kf.Estimate()(1), 5, 0.01);
    EXPECT_NEAR(kf.Predict()(0), 10, 0.01);
    EXPECT_NEAR(kf.Predict()(1), 5, 0.01);
}

TEST(KalmanFilterTest, HypothesisTest) {
    const double initial{0.};
    const double drift{0.5};
    const double noise{0.1};
    const auto stateModel{
        StochasticModels::CreateRandomWalk(Eigen::MatrixXd{{drift}})};
    const auto obsModel{
        StochasticModels::CreateRandomWalk(Eigen::MatrixXd{{noise}})};

    KalmanFilter kf{Eigen::MatrixXd{{initial}}, Eigen::MatrixXd{{1}},
                    stateModel, obsModel};

    const unsigned ITER{10000};
    std::vector<double> innovations(ITER);
    std::vector<double> normInnovations(ITER);
    auto hiddenState = initial + Common::SampleNormal() * std::sqrt(drift);

    for (unsigned i = 0; i < ITER; ++i) {
        // generate fake obs
        auto obs = hiddenState + Common::SampleNormal() * std::sqrt(noise);
        kf.Update(Eigen::VectorXd{{obs}});

        // store data
        auto [innovation, innVar] = kf.GetLastInnovation();
        innovations.emplace_back(innovation(0, 0));
        normInnovations.emplace_back(std::pow(innovation(0, 0), 2) /
                                     innVar(0, 0));
        hiddenState += Common::SampleNormal() * std::sqrt(drift);
    }

    // Chi-squared Test
    const auto innovationSum =
        std::accumulate(normInnovations.begin(), normInnovations.end(), 0.);
    EXPECT_NEAR(innovationSum / ITER, 1.1, 0.1);

    // Auto-Correlation Test
    alglib::real_1d_array v1;
    v1.setcontent(innovations.size() - 1, &(innovations[0]));
    alglib::real_1d_array v2;
    v2.setcontent(innovations.size() - 1, &(innovations[1]));
    EXPECT_NEAR(alglib::pearsoncorr2(v1, v2), 0., 0.1);
}

}  // namespace Filters::Test
