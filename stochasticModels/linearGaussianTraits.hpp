#pragma once
#include <Eigen/Dense>
#include <type_traits>

namespace StochasticModels {

template <int Input, int Output>
class StaticLGTraits {
   public:
    using CoefMatrix = Eigen::Matrix<double, Output, Input>;
    using NoiseMatrix = Eigen::Matrix<double, Output, Output>;
    using InputVector = Eigen::Vector<double, Input>;
    using OutputVector = Eigen::Vector<double, Output>;
};

class DynamicLGTraits {
   public:
    using CoefMatrix = Eigen::MatrixXd;
    using NoiseMatrix = Eigen::MatrixXd;
    using InputVector = Eigen::VectorXd;
    using OutputVector = Eigen::VectorXd;
};

}  // namespace StochasticModels
