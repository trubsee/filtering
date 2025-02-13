#include <Eigen/Dense>

#include "spline/bspline.hpp"

namespace Spline {

template <unsigned int degree>
BSpline<degree> BSpline<degree>::WeightedFit(
    const std::vector<std::pair<double, double>>& data,
    const std::vector<double>& weights, const std::vector<double>& knots) {
    const unsigned int n = knots.size() - degree - 1;

    Eigen::MatrixXd N(data.size(), n);
    Eigen::VectorXd Y(data.size());

    for (std::size_t i = 0; i < data.size(); ++i) {
        for (unsigned int j = 0; j < n; ++j)
            N(i, j) = BasisFunction(knots, degree, j, data[i].first);
        Y(i) = data[i].second * weights[i];
    }
    Eigen::VectorXd controlPoints = N.colPivHouseholderQr().solve(Y);
    return BSpline<degree>(
        knots,
        std::vector<double>(controlPoints.data(),
                            controlPoints.data() + controlPoints.size()));
}

template <unsigned int degree>
BSpline<degree> BSpline<degree>::Fit(
    const std::vector<std::pair<double, double>>& data,
    const std::vector<double>& knots) {
    const std::vector<double> weights(data.size(), 1);
    return WeightedFit(data, weights, knots);
}

}  // namespace Spline
