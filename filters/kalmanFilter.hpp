#pragma once

#include <Eigen/Dense>

#include "common/assert.hpp"

namespace Filters {

template<int hidden, int observed>
class BasicKF
{
public:
    using HiddenMatrix = Eigen::Matrix<double, hidden, hidden>;
    using HiddenVector = Eigen::Vector<double, hidden>;
    using ObservedMatrix = Eigen::Matrix<double, observed, observed>;
    using ObservedVector = Eigen::Vector<double, observed>;
    
    BasicKF(
        const HiddenVector& stateEstimate,
        const HiddenMatrix& covEstimate,
        const HiddenMatrix& stateTransition,
        const Eigen::Matrix<double, observed, hidden>& observationModel,
        const HiddenMatrix& processNoise,
        const ObservedMatrix& observationNoise
    )
    :
        x{stateEstimate},
        P{covEstimate},
        F{stateTransition},
        H{observationModel},
        Q{processNoise},
        R{observationNoise}
    {
    }

    void Update(const ObservedVector& obs);

    const HiddenVector& Estimate() const { return x; }

    const HiddenVector Predict() const { return F * x; }

    std::pair<const HiddenVector&, const HiddenMatrix&> GetLastInnovation() const { return {y, S}; }

private:
    // current estimate
    HiddenVector x;
    HiddenMatrix P;
    // innovations
    ObservedVector y;
    ObservedMatrix S;

    // model params
    HiddenMatrix F;
    Eigen::Matrix<double, observed, hidden> H;
    HiddenMatrix Q;
    ObservedMatrix R;
};

}
