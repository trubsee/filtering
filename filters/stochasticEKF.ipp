#include "common/statistics.hpp"
#include "filters/stochasticEKF.hpp"

namespace Filters {

template <typename Traits, int NumSamples>
StochasticEKFImpl<Traits, NumSamples>::StochasticEKFImpl(
    const HiddenVector& stateEstimate, const HiddenMatrix& covEstimate,
    const HiddenMatrix& f, const HiddenMatrix& q, const ObsMatrix& h,
    const ObsNoiseMatrix& r)
    : mStateModel{f, q},
      H{h},
      R{r},
      mSamples{q.rows(), NumSamples},
      y{r.rows()},
      S{r.rows(), r.rows()} {
    // ASSERT(stateEstimate.size() == Hidden);
    // ASSERT(covEstimate.rows() == covEstimate.cols());
    // ASSERT(covEstimate.rows() == Hidden);
    // ASSERT(stateModel.GetNumOutputs() == stateModel.GetNumInputs());
    // ASSERT(stateModel.GetNumOutputs() == Hidden);
    InitialiseSamples(stateEstimate, covEstimate);
}

template <typename Traits, int NumSamples>
StochasticEKFImpl<Traits, NumSamples>::StochasticEKFImpl(
    const HiddenVector& stateEstimate, const HiddenMatrix& covEstimate,
    const HiddenModel& stateModel, const ObservedModel& obsModel)
    : StochasticEKFImpl{stateEstimate,
                        covEstimate,
                        stateModel.GetCoefMatrix(),
                        stateModel.GetNoiseMatrix(),
                        obsModel.GetCoefMatrix(),
                        obsModel.GetNoiseMatrix()} {}

template <typename Traits, int NumSamples>
void StochasticEKFImpl<Traits, NumSamples>::Update(const ObsVector& obs) {
    ASSERT(obs.size() == y.size());

    // Predict
    const auto xPred = Predict();
    const HiddenMatrix PPred =
        Common::CalculateCovariance(mSamples.transpose(), mSamples.transpose());

    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    mEstimate = xPred + K * y;

    UpdateSamples(obs, K);
    mPrediction = std::nullopt;
}

template <typename Traits, int NumSamples>
const StochasticEKFImpl<Traits, NumSamples>::HiddenVector&
StochasticEKFImpl<Traits, NumSamples>::Predict() {
    if (mPrediction) return *mPrediction;

    Eigen::VectorXd sum{mEstimate.size()};
    for (unsigned i = 0; i < NumSamples; ++i) {
        auto newSample = mStateModel.Mutate(mSamples.col(i));
        mSamples.block(0, i, mEstimate.size(), 1) = newSample;
        sum += newSample;
    }
    mPrediction = sum / NumSamples;
    return *mPrediction;
}

template <typename Traits, int NumSamples>
void StochasticEKFImpl<Traits, NumSamples>::InitialiseSamples(
    const HiddenVector& stateEstimate, const HiddenMatrix& covEstimate) {
    const HiddenMatrix tril = covEstimate.llt().matrixL();
    for (unsigned i = 0; i < NumSamples; ++i) {
        mSamples.block(0, i, mEstimate.size(), 1) =
            stateEstimate + Common::SampleMvNormal(tril);
    }
}

template <typename Traits, int NumSamples>
void StochasticEKFImpl<Traits, NumSamples>::UpdateSamples(const ObsVector& obs,
                                                          const ObsMatrixT& K) {
    const ObsNoiseMatrix tril = R.llt().matrixL();
    for (unsigned i = 0; i < NumSamples; ++i) {
        const HiddenVector x_i = mSamples.col(i);
        const auto v = Common::SampleMvNormal(tril);
        mSamples.block(0, i, mEstimate.size(), 1) =
            x_i + K * (obs + v - H * x_i);
    }
}

}  // namespace Filters
