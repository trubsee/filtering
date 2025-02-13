#include <cmath>
#include <numeric>
#include <random>

#include "common/randomSample.hpp"
#include "filters/stochasticEKF.ipp"
#include "gtest/gtest.h"
#include "libalglib/statistics.h"
#include "stochasticModels/linearFactoryMethods.hpp"

namespace Filters::Test {

TEST(StochasticEKFTest, CheckZeroDriftStatic) {
    const auto stateModel{
        StochasticModels::CreateRandomWalk<2>(Eigen::Matrix2d{{0, 0}, {0, 0}})};
    const auto obsModel{StochasticModels::CreateRandomWalk<2>(
        Eigen::Matrix2d{{0.5, 0}, {0, 2}})};

    StochasticEKF::Static<2, 2, 500> ekf{Eigen::Vector2d{1, 10},
                                         Eigen::Matrix2d{{1, 0}, {0, 10}},
                                         stateModel,
                                         obsModel};

    for (unsigned i = 0; i < 1000; ++i) {
        ekf.Update(Eigen::Vector2d{1, 12});
    }
    EXPECT_NEAR(ekf.Estimate()(0), 1, 0.1);
    EXPECT_NEAR(ekf.Estimate()(1), 12, 0.1);
    EXPECT_NEAR(ekf.Predict()(0), 1, 0.1);
    EXPECT_NEAR(ekf.Predict()(1), 12, 0.1);
}

TEST(StochasticEKFTest, CheckZeroDriftDynamic) {
    const auto stateModel{
        StochasticModels::CreateRandomWalk(Eigen::Matrix2d{{0, 0}, {0, 0}})};
    const auto obsModel{
        StochasticModels::CreateRandomWalk(Eigen::Matrix2d{{0.5, 0}, {0, 2}})};

    StochasticEKF::Dynamic ekf{Eigen::Vector2d{1, 10},
                               Eigen::Matrix2d{{1, 0}, {0, 10}},
                               stateModel,
                               obsModel};

    for (unsigned i = 0; i < 1000; ++i) {
        ekf.Update(Eigen::Vector2d{1, 12});
    }
    EXPECT_NEAR(ekf.Estimate()(0), 1, 0.1);
    EXPECT_NEAR(ekf.Estimate()(1), 12, 0.1);
    EXPECT_NEAR(ekf.Predict()(0), 1, 0.1);
    EXPECT_NEAR(ekf.Predict()(1), 12, 0.1);
}

TEST(StochasticEKFTest, ObsMoreThanHidden) {
    const auto stateModel{StochasticModels::CreateRandomWalk<2>(
        Eigen::Matrix2d{{0.1, 0}, {0, 0.1}})};
    const StochasticModels::LinearGaussian::Static<2, 3> obsModel{
        Eigen::MatrixXd{{1, 0}, {1, 0}, {0, 1}},
        Eigen::Matrix3d{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};

    StochasticEKF::Static<2, 3, 500> ekf{Eigen::Vector2d{0, 0},
                                         Eigen::Matrix2d{{1, 0}, {0, 1}},
                                         stateModel,
                                         obsModel};

    for (unsigned i = 0; i < 1000; ++i) {
        ekf.Update(Eigen::Vector3d{10.1, 9.9, 5.});
    }
    EXPECT_NEAR(ekf.Estimate()(0), 10, 0.1);
    EXPECT_NEAR(ekf.Estimate()(1), 5, 0.1);
}

TEST(StochasticEKFTest, HypothesisTest) {
    const double initial{0.};
    const double drift{0.5};
    const double noise{0.1};
    const auto stateModel{
        StochasticModels::CreateRandomWalk<1>(Eigen::MatrixXd{{drift}})};
    const auto obsModel{
        StochasticModels::CreateRandomWalk<1>(Eigen::MatrixXd{{noise}})};

    StochasticEKF::Static<1, 1, 500> ekf{
        Eigen::MatrixXd{{initial}}, Eigen::MatrixXd{{1}}, stateModel, obsModel};

    const unsigned ITER{1000};
    std::vector<double> innovations(ITER);
    std::vector<double> normInnovations(ITER);
    auto hiddenState = initial + Common::SampleNormal() * std::sqrt(drift);

    for (unsigned i = 0; i < ITER; ++i) {
        // generate fake obs
        auto obs = hiddenState + Common::SampleNormal() * std::sqrt(noise);
        ekf.Update(Eigen::VectorXd{{obs}});

        // store data
        auto [innovation, innVar] = ekf.GetLastInnovation();
        innovations.emplace_back(innovation(0, 0));
        normInnovations.emplace_back(std::pow(innovation(0, 0), 2) /
                                     innVar(0, 0));
        hiddenState += Common::SampleNormal() * std::sqrt(drift);
    }

    // Chi-squared Test
    const auto innovationSum =
        std::accumulate(normInnovations.begin(), normInnovations.end(), 0.);
    EXPECT_NEAR(innovationSum / ITER, 1., 0.1);

    // Auto-Correlation Test
    alglib::real_1d_array v1;
    v1.setcontent(innovations.size() - 1, &(innovations[0]));
    alglib::real_1d_array v2;
    v2.setcontent(innovations.size() - 1, &(innovations[1]));
    EXPECT_NEAR(alglib::pearsoncorr2(v1, v2), 0., 0.1);
}

}  // namespace Filters::Test
