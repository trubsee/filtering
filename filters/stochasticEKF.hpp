#pragma once

#include <Eigen/Dense>
#include <optional>

#include "filters/stateSpaceModelTraits.hpp"
#include "stochasticModels/linearGaussian.hpp"

namespace Filters {

template <int Hidden, int Observed, int NumSamples = 1000>
class StochasticEKF {
    using Traits = StateSpaceModelTraits<Hidden, Observed>;
    using HiddenMatrix = typename Traits::HiddenMatrix;
    using ObsMatrix = typename Traits::ObsMatrix;
    using ObsMatrixT = typename Traits::ObsMatrixT;
    using ObsNoiseMatrix = typename Traits::ObsNoiseMatrix;
    using HiddenVector = typename Traits::HiddenVector;
    using ObsVector = typename Traits::ObsVector;
    using HiddenModel = typename Traits::HiddenModel;
    using ObservedModel = typename Traits::ObservedModel;
    using SampleMatrix = typename std::conditional_t <
                         Observed<20, Eigen::Matrix<double, Hidden, NumSamples>,
                                  Eigen::MatrixXd>;

   public:
    StochasticEKF(const HiddenVector&, const HiddenMatrix&,
                  const HiddenModel&,
                  const ObservedModel&);

    void Update(const ObsVector&);

    const HiddenVector& Predict();

    const HiddenVector& Estimate() const { return mEstimate; }

    std::pair<const ObsVector&, const ObsNoiseMatrix&> GetLastInnovation()
        const {
        return {y, S};
    }

   private:
    void InitialiseSamples(const HiddenVector&, const HiddenMatrix&);

    void UpdateSamples(const ObsVector&, const ObsMatrixT&);

    // model params
    const HiddenModel& mStateModel;
    ObsMatrix H;
    ObsNoiseMatrix R;

    // estimate samples
    SampleMatrix mSamples;
    std::optional<HiddenVector> mPrediction;
    HiddenVector mEstimate;
    // innovation
    ObsVector y;
    ObsNoiseMatrix S;
};

}  // namespace Filters
