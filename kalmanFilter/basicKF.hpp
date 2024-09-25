#pragma once

#include <Eigen/Dense>

namespace KalmanFilter {

class BasicKF
{
public:
    BasicKF(
        const Eigen::MatrixXd& stateEstimate,
        const Eigen::MatrixXd& covEstimate,
        const Eigen::MatrixXd& stateTransition,
        const Eigen::MatrixXd& observationModel,
        const Eigen::MatrixXd& processNoise,
        const Eigen::MatrixXd& observationNoise
    )
    :
        x{stateEstimate},
        P{covEstimate},
        F{stateTransition},
        H{observationModel},
        Q{processNoise},
        R{observationNoise}
    {
    }

    void Update(const Eigen::MatrixXd& obs);

    const Eigen::MatrixXd& Estimate() const { return x; }

    const Eigen::MatrixXd Predict() const { return F * x; }

private:
    Eigen::MatrixXd x;
    Eigen::MatrixXd P;

    Eigen::MatrixXd F;
    Eigen::MatrixXd H;
    Eigen::MatrixXd Q;
    Eigen::MatrixXd R;
};

}
