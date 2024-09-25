#include "gtest/gtest.h"

#include "kalmanFilter/basicKF.hpp"

namespace KalmanFilter {

TEST(KalmanFilterTest, CheckUpdate)
{
    BasicKF kf {
        Eigen::MatrixXd{{0}},
        Eigen::MatrixXd{{1}},
        Eigen::MatrixXd{{1}},
        Eigen::MatrixXd{{1}},
        Eigen::MatrixXd{{0.5}},
        Eigen::MatrixXd{{0.5}}
    };
}

}
