#pragma once
#include <vector>
#include <iostream>

#include "common/assert.hpp"


namespace Spline {

template <unsigned int degree>
class BSpline
{
public:
    BSpline(const std::vector<double>& knots, const std::vector<double>& controlPoints)
    :
        mKnots{knots},
        mControlPoints{controlPoints}
    {
        ASSERT(controlPoints.size() + degree + 1 == knots.size());
    }

    double Evaluate(double x) const {
        if (x <= mKnots[0]) return mControlPoints[0];
        if (x >= mKnots[mKnots.size()-1]) return mControlPoints[mControlPoints.size()-1];
        return DeBoor(FindSpan(mKnots, x), x); 
    }

    static BSpline WeightedFit(const std::vector<std::pair<double, double>>&, const std::vector<double>&, const std::vector<double>&);
    
    static BSpline Fit(const std::vector<std::pair<double, double>>&, const std::vector<double>&);
   
private:
    static unsigned int FindSpan(const std::vector<double>& knots, double x)
    {
        ASSERT(x > knots[0] && x < knots[knots.size()-1]); 
        auto it = std::lower_bound(knots.begin() + degree, knots.end() - degree, x);
        return (unsigned int)(it - knots.begin() - 1);
    }

    static double BasisFunction(const std::vector<double>& knots, unsigned int k, unsigned int i, double x)
    {
        if (k == 0)
            return (knots[i] <= x && x < knots[i + 1]) ? 1. : 0.;

        double answer {0};
        if (knots[i+k] != knots[i])
        {
            const double left = (x - knots[i]) / (knots[i + k] - knots[i]);
            answer += left * BasisFunction(knots, k-1, i, x);
        }
        if (knots[i+k+1] != knots[i+1])
        {
            const double right = (knots[i + k + 1] - x) / (knots[i + k + 1] - knots[i + 1]);
            answer += right * BasisFunction(knots, k-1, i+1, x);
        }
        return answer;
    }

    double DeBoor(unsigned int i, double x) const
    {
        std::vector<double> d(mControlPoints.begin() + i - degree, mControlPoints.begin() + i + 1);
        for (unsigned int r = 1; r <= degree; ++r)
        {
            for (unsigned int j = degree; j >= r; --j)
            {
                const double alpha = (x - mKnots[i - degree + j]) / (mKnots[i + 1 + j - r] - mKnots[i - degree + j]);
                d[j] = (1. - alpha) * d[j-1] + alpha * d[j];
            }
        }
        return d[degree];
    }
    
    std::vector<double> mKnots;
    std::vector<double> mControlPoints;
};

}
