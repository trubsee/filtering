#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <optional>

#include "filters/stateSpaceModelTraits.hpp"
#include "stochasticModels/linearGaussian.hpp"

namespace Filters {

template <typename Traits, int NumSamples>
class StochasticEKFImpl {
   public:
    using HiddenMatrix = typename Traits::HiddenMatrix;
    using ObsMatrix = typename Traits::ObsMatrix;
    using ObsMatrixT = typename Traits::ObsMatrixT;
    using ObsNoiseMatrix = typename Traits::ObsNoiseMatrix;
    using HiddenVector = typename Traits::HiddenVector;
    using ObsVector = typename Traits::ObsVector;
    using HiddenModel = typename Traits::HiddenModel;
    using ObservedModel = typename Traits::ObservedModel;
    using SampleMatrix = typename Traits::SampleMatrix<NumSamples>;

    StochasticEKFImpl(const HiddenVector&, const HiddenMatrix&,
                      const HiddenMatrix&, const HiddenMatrix&,
                      const ObsMatrix&, const ObsNoiseMatrix&);

    StochasticEKFImpl(const HiddenVector&, const HiddenMatrix&,
                      const HiddenModel&, const ObservedModel&);

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
    HiddenModel mStateModel;
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

struct StochasticEKF {
    template <int Hidden, int Observed, int NumSamples>
    using Static =
        StochasticEKFImpl<StaticSSMTraits<Hidden, Observed>, NumSamples>;

    using Dynamic = StochasticEKFImpl<DynamicSSMTraits, 200>;
};

}  // namespace Filters
