#include "common/assert.hpp" 
#include "filters/kalmanFilter.hpp"

namespace Filters {

KalmanFilter::KalmanFilter(
    const Eigen::VectorXd& stateEstimate,
    const Eigen::MatrixXd& covEstimate,
    const StochasticModels::LinearGaussian& stateModel,
    const StochasticModels::LinearGaussian& obsModel
)
:
    F{stateModel.GetCoefMatrix()},
    H{obsModel.GetCoefMatrix()},
    Q{stateModel.GetNoiseMatrix()},
    R{obsModel.GetNoiseMatrix()},
    mHidden{(unsigned)H.cols()},
    mObserved{(unsigned)H.rows()},
    x{stateEstimate},
    P{covEstimate},
    y{mObserved},
    S{mObserved, mObserved}
{
    ASSERT(stateEstimate.size() == mHidden);
    ASSERT(covEstimate.rows() == covEstimate.cols());
    ASSERT(covEstimate.rows() == mHidden);
    ASSERT(stateModel.GetNumOutputs() == stateModel.GetNumInputs());
    ASSERT(stateModel.GetNumOutputs() == mHidden);
}

void KalmanFilter::Update(const Eigen::VectorXd& obs)
{
    ASSERT(obs.size() == mObserved);

    // Predict 
    const auto xPred = Predict();
    const auto PPred = F.transpose() * P * F + Q;
   
    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    x = xPred + K * y;
    P = (Eigen::MatrixXd::Identity(mHidden, mHidden) - K * H) * P;
}

}
