#pragma once

#include <Eigen/Dense>

#include "common/assert.hpp"

namespace Filters {

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
    );

    void Update(const Eigen::MatrixXd& obs);

    const Eigen::MatrixXd& Estimate() const { return x; }

    const Eigen::MatrixXd Predict() const { return F * x; }

    std::pair<const Eigen::MatrixXd&, const Eigen::MatrixXd&> GetLastInnovation() const
    {
        ASSERT(y.cols() == 1 && y.rows() == x.rows());
        ASSERT(S.cols() == y.rows() && S.rows() == y.rows());
        return {y, S};
    }

private:
    // current estimate
    Eigen::MatrixXd x;
    Eigen::MatrixXd P;
    // innovations
    Eigen::MatrixXd y;
    Eigen::MatrixXd S;

    // model params
    Eigen::MatrixXd F;
    Eigen::MatrixXd H;
    Eigen::MatrixXd Q;
    Eigen::MatrixXd R;
};

}
