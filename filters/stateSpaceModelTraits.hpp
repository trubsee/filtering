#pragma once
#include <Eigen/Dense>
#include <type_traits>

#include "stochasticModels/linearGaussian.hpp"

namespace Filters {

template <int Hidden, int Observed>
class StaticSSMTraits {
   public:
    using HiddenMatrix = Eigen::Matrix<double, Hidden, Hidden>;
    using ObsMatrix = Eigen::Matrix<double, Observed, Hidden>;
    using ObsMatrixT = Eigen::Matrix<double, Hidden, Observed>;
    using ObsNoiseMatrix = Eigen::Matrix<double, Observed, Observed>;
    using HiddenVector = Eigen::Vector<double, Hidden>;
    using ObsVector = Eigen::Vector<double, Observed>;
    template <int NumSamples>
    using SampleMatrix = Eigen::Matrix<double, Hidden, NumSamples>;

    using HiddenModel =
        StochasticModels::LinearGaussian::Static<Hidden, Hidden>;
    using ObservedModel =
        StochasticModels::LinearGaussian::Static<Hidden, Observed>;

    static HiddenMatrix Identity(int = 0) {
        return HiddenMatrix::Identity(Hidden, Hidden);
    }
};

class DynamicSSMTraits {
   public:
    using HiddenMatrix = Eigen::MatrixXd;
    using ObsMatrix = Eigen::MatrixXd;
    using ObsMatrixT = Eigen::MatrixXd;
    using ObsNoiseMatrix = Eigen::MatrixXd;
    using HiddenVector = Eigen::VectorXd;
    using ObsVector = Eigen::VectorXd;
    template <int NumSamples>
    using SampleMatrix = Eigen::MatrixXd;

    using HiddenModel = StochasticModels::LinearGaussian::Dynamic;
    using ObservedModel = StochasticModels::LinearGaussian::Dynamic;

    static HiddenMatrix Identity(int size) {
        return HiddenMatrix::Identity(size, size);
    }
};

template <int Hidden, int Observed>
class StateSpaceModelTraits {
   public:
    using HiddenMatrix =
        std::conditional_t <
        Hidden<20, Eigen::Matrix<double, Hidden, Hidden>, Eigen::MatrixXd>;

    using ObsMatrix =
        std::conditional_t < Observed < 20 ||
        Hidden<20, Eigen::Matrix<double, Observed, Hidden>, Eigen::MatrixXd>;

    using ObsMatrixT =
        std::conditional_t < Observed < 20 ||
        Hidden<20, Eigen::Matrix<double, Hidden, Observed>, Eigen::MatrixXd>;

    using ObsNoiseMatrix =
        std::conditional_t <
        Observed<20, Eigen::Matrix<double, Observed, Observed>,
                 Eigen::MatrixXd>;

    using HiddenVector =
        std::conditional_t <
        Hidden<20, Eigen::Vector<double, Hidden>, Eigen::VectorXd>;

    using ObsVector =
        std::conditional_t <
        Observed<20, Eigen::Vector<double, Observed>, Eigen::VectorXd>;

    using HiddenModel = StochasticModels::LinearGaussian::Dynamic;

    using ObservedModel = StochasticModels::LinearGaussian::Dynamic;
};

}  // namespace Filters
