#pragma once
#include <Eigen/Dense>
#include <type_traits>

#include "stochasticModels/linearGaussian.hpp"

namespace Filters {

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

    using HiddenModel = StochasticModels::LinearGaussian<Hidden, Hidden>;

    using ObservedModel = StochasticModels::LinearGaussian<Hidden, Observed>;
};

}  // namespace Filters
