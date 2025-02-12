#include <vector>

#include "gtest/gtest.h"
#include "spline/bspline.ipp"

namespace Spline::Test {

TEST(BSplineTest, TestFit) {
    std::vector<std::pair<double, double>> data{
        {0., 0.}, {1., 2.}, {2., 3.}, {3., 5.}, {3.9, 4.}};
    std::vector<double> knots = {
        0.0, 0.0, 0.0, 0.0, 1.5, 2.5, 4.0, 4.0, 4.0, 4.0};
    auto spline = BSpline<3>::Fit(data, knots);

    for (std::size_t i = 0; i < data.size(); ++i) {
        const double result = spline.Evaluate(data[i].first);
        EXPECT_NEAR(result, data[i].second, 1e-6);
    }
}

}  // namespace Spline::Test
