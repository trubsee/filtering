#include "kalmanFilter/basicKF.hpp"

namespace KalmanFilter {

void BasicKF::Update(const Eigen::MatrixXd& obs)
{
    // Predict 
    const auto xPred = F * x;
    const auto PPred = F * P * F.transpose();
   
    // Update
    const auto y = obs - H * xPred;
    const auto S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    x = xPred + K * y;
    P = (Eigen::MatrixXd::Identity(P.rows(), P.cols()) - K * H) * P;
}

}
