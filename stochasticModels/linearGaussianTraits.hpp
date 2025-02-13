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

template <int Input, int Output>
class LinearGaussianTraits {
   public:
    using CoefMatrix =
        std::conditional_t < Input < 20 ||
        Output<20, Eigen::Matrix<double, Output, Input>, Eigen::MatrixXd>;

    using NoiseMatrix =
        std::conditional_t <
        Output<20, Eigen::Matrix<double, Output, Output>, Eigen::MatrixXd>;

    using InputVector =
        std::conditional_t <
        Input<20, Eigen::Vector<double, Input>, Eigen::VectorXd>;

    using OutputVector =
        std::conditional_t <
        Output<20, Eigen::Vector<double, Output>, Eigen::VectorXd>;
};

}  // namespace StochasticModels
