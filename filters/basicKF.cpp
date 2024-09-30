#include "filters/basicKF.hpp"

namespace Filters {

BasicKF::BasicKF(
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
    ASSERT(x.cols() == 1);
    const auto n_hidden = x.rows();
    ASSERT(P.cols() == n_hidden && P.rows() == n_hidden);

    ASSERT(F.cols() == n_hidden && F.rows() == n_hidden);
    ASSERT(Q.cols() == n_hidden && Q.rows() == n_hidden)

    ASSERT(H.cols() == n_hidden);
    const auto n_obs = H.rows();
    ASSERT(R.cols() == n_obs && R.rows() == n_obs);
}


void BasicKF::Update(const Eigen::MatrixXd& obs)
{
    // Predict 
    const auto xPred = F * x;
    const auto PPred = F * P * F.transpose() + Q;
   
    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    x = xPred + K * y;
    P = (Eigen::MatrixXd::Identity(P.rows(), P.cols()) - K * H) * P;
}

}
