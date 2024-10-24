#include "common/statistics.hpp"
#include "filters/stochasticEKF.hpp"

namespace Filters {

template <int numSamples>
StochasticEKF<numSamples>::StochasticEKF(
    const Eigen::VectorXd& stateEstimate,
    const Eigen::MatrixXd& covEstimate,
    const StochasticModels::LinearGaussian& stateModel,
    const StochasticModels::LinearGaussian& obsModel
)
:
    mStateModel{stateModel},
    H{obsModel.GetCoefMatrix()},
    R{obsModel.GetNoiseMatrix()},
    mHidden{(unsigned)H.cols()},
    mObserved{(unsigned)H.rows()},
    mSamples{mHidden, numSamples},
    y{mObserved},
    S{mObserved, mObserved}
{
    ASSERT(stateEstimate.size() == mHidden);
    ASSERT(covEstimate.rows() == covEstimate.cols());
    ASSERT(covEstimate.rows() == mHidden);
    ASSERT(stateModel.GetNumOutputs() == stateModel.GetNumInputs());
    ASSERT(stateModel.GetNumOutputs() == mHidden);
    InitialiseSamples(stateEstimate, covEstimate);
}

template <int numSamples>
void StochasticEKF<numSamples>::Update(const Eigen::VectorXd& obs)
{
    ASSERT(obs.size() == mObserved);

    // Predict
    const auto xPred = Predict();
    const auto PPred = Common::CalculateCovariance(mSamples.transpose(), mSamples.transpose());

    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    mEstimate = xPred + K * y;
    
    const Eigen::MatrixXd tril = R.llt().matrixL();
    for (unsigned i = 0; i < numSamples; ++i)
    {
        const auto x_i = mSamples.col(i);
        const auto v = Common::SampleMvNormal(tril);
        mSamples.block(0, i, mHidden, 1) = x_i + K * (obs + v - H * x_i);
    }
    mPrediction = std::nullopt;
}

template <int numSamples>
const Eigen::VectorXd& StochasticEKF<numSamples>::Predict()
{
    if (mPrediction)
        return *mPrediction;

    Eigen::VectorXd sum{mHidden};
    for (unsigned i = 0; i < numSamples; ++i)
    {
        auto newSample = mStateModel.Mutate(mSamples.col(i));
        mSamples.block(0, i, mHidden, 1) = newSample;
        sum += newSample;
    }
    mPrediction = sum / numSamples;
    return *mPrediction;
}

template <int numSamples>
void StochasticEKF<numSamples>::InitialiseSamples(
    const Eigen::VectorXd& stateEstimate,
    const Eigen::MatrixXd& covEstimate)
{
    const Eigen::MatrixXd tril = covEstimate.llt().matrixL();
    for (unsigned i = 0; i < numSamples; ++i)
    {
        mSamples.block(0, i, mHidden, 1) = stateEstimate + Common::SampleMvNormal(tril);
    }
}

}
