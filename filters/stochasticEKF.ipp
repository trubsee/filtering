#include "common/statistics.hpp"
#include "filters/stochasticEKF.hpp"

namespace Filters {

template <int Hidden, int Observed, int NumSamples>
StochasticEKF<Hidden, Observed, NumSamples>::StochasticEKF(
    const HiddenVector& stateEstimate,
    const HiddenMatrix& covEstimate,
    const HiddenModel& stateModel,
    const ObservedModel& obsModel
)
:
    mStateModel{stateModel},
    H{obsModel.GetCoefMatrix()},
    R{obsModel.GetNoiseMatrix()},
    mSamples{Hidden, NumSamples},
    y{Observed},
    S{Observed, Observed}
{
    ASSERT(stateEstimate.size() == Hidden);
    ASSERT(covEstimate.rows() == covEstimate.cols());
    ASSERT(covEstimate.rows() == Hidden);
    ASSERT(stateModel.GetNumOutputs() == stateModel.GetNumInputs());
    ASSERT(stateModel.GetNumOutputs() == Hidden);
    InitialiseSamples(stateEstimate, covEstimate);
}

template <int Hidden, int Observed, int NumSamples>
void StochasticEKF<Hidden, Observed, NumSamples>::Update(const ObsVector& obs)
{
    ASSERT(obs.size() == Observed);

    // Predict
    const auto xPred = Predict();
    const HiddenMatrix PPred = Common::CalculateCovariance(mSamples.transpose(), mSamples.transpose());

    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    mEstimate = xPred + K * y;
   
    UpdateSamples(obs, K);
    mPrediction = std::nullopt;
}

template <int Hidden, int Observed, int NumSamples>
const StochasticEKF<Hidden, Observed, NumSamples>::HiddenVector& StochasticEKF<Hidden, Observed, NumSamples>::Predict()
{
    if (mPrediction)
        return *mPrediction;

    Eigen::VectorXd sum{Hidden};
    for (unsigned i = 0; i < NumSamples; ++i)
    {
        auto newSample = mStateModel.Mutate(mSamples.col(i));
        mSamples.block(0, i, Hidden, 1) = newSample;
        sum += newSample;
    }
    mPrediction = sum / NumSamples;
    return *mPrediction;
}

template <int Hidden, int Observed, int NumSamples>
void StochasticEKF<Hidden, Observed, NumSamples>::InitialiseSamples(
    const HiddenVector& stateEstimate,
    const HiddenMatrix& covEstimate)
{
    const HiddenMatrix tril = covEstimate.llt().matrixL();
    for (unsigned i = 0; i < NumSamples; ++i)
    {
        mSamples.block(0, i, Hidden, 1) = stateEstimate + Common::SampleMvNormal(tril);
    }
}


template <int Hidden, int Observed, int NumSamples>
void StochasticEKF<Hidden, Observed, NumSamples>::UpdateSamples(const ObsVector& obs, const ObsMatrixT& K)
{
    const ObsNoiseMatrix tril = R.llt().matrixL();
    for (unsigned i = 0; i < NumSamples; ++i)
    {
        const HiddenVector x_i = mSamples.col(i);
        const auto v = Common::SampleMvNormal(tril);
        mSamples.block(0, i, Hidden, 1) = x_i + K * (obs + v - H * x_i);
    }
}

}
