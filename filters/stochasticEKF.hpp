#pragma once

#include <Eigen/Dense>
#include <optional>

#include "stochasticModels/linearGaussian.hpp"

namespace Filters {

template <int numSamples = 1000>
class StochasticEKF {
   public:
    StochasticEKF(const Eigen::VectorXd&, const Eigen::MatrixXd&,
                  const StochasticModels::LinearGaussian&,
                  const StochasticModels::LinearGaussian&);

    void Update(const Eigen::VectorXd&);

    const Eigen::VectorXd& Predict();

    const Eigen::VectorXd& Estimate() const { return mEstimate; }

    std::pair<const Eigen::VectorXd&, const Eigen::MatrixXd&>
    GetLastInnovation() const {
        return {y, S};
    }

   private:
    void InitialiseSamples(const Eigen::VectorXd&, const Eigen::MatrixXd&);

    void UpdateSamples(const Eigen::VectorXd&, const Eigen::MatrixXd&);

    // model params
    const StochasticModels::LinearGaussian& mStateModel;
    Eigen::MatrixXd H;
    Eigen::MatrixXd R;

    // dimensions
    const unsigned mHidden;
    const unsigned mObserved;

    // estimate samples
    Eigen::MatrixXd mSamples;
    std::optional<Eigen::VectorXd> mPrediction;
    Eigen::VectorXd mEstimate;
    // innovation
    Eigen::VectorXd y;
    Eigen::MatrixXd S;
};

}  // namespace Filters
