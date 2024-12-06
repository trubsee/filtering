#pragma once
#include <Eigen/Dense>
#include <type_traits>

namespace Filters {

template <int Hidden, int Observed>
class StateSpaceModelTraits {
   public:
    using HiddenMatrix =
        std::conditional_t <
        Hidden<20, Eigen::Matrix<double, Hidden, Hidden>, Eigen::MatrixXd>;

    using ObsMatrix =
        typename std::conditional_t < Observed < 20 ||
        Hidden<20, Eigen::Matrix<double, Observed, Hidden>, Eigen::MatrixXd>;
    
    using ObsMatrixT =
        typename std::conditional_t < Observed < 20 ||
        Hidden<20, Eigen::Matrix<double, Hidden, Observed>, Eigen::MatrixXd>;

    using ObsNoiseMatrix =
        typename std::conditional_t <
        Observed<20, Eigen::Matrix<double, Observed, Observed>,
                 Eigen::MatrixXd>;

    using HiddenVector =
        typename std::conditional_t <
        Hidden<20, Eigen::Vector<double, Hidden>, Eigen::VectorXd>;

    using ObsVector =
        typename std::conditional_t <
        Observed<20, Eigen::Vector<double, Observed>, Eigen::VectorXd>;
};

}  // namespace Filters
