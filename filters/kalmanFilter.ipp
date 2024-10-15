#include "filters/kalmanFilter.hpp"

namespace Filters {

template<int hidden, int observed>
void BasicKF<hidden, observed>::Update(const ObservedVector& obs)
{
    // Predict 
    const auto xPred = Predict();
    const auto PPred = F.transpose() * P * F + Q;
   
    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    x = xPred + K * y;
    P = (HiddenMatrix::Identity() - K * H) * P;
}

}
