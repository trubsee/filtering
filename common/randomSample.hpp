#pragma once

#include <random>

namespace Common {

namespace {

    std::random_device mRandomDevice;
    std::mt19937 mGen{ mRandomDevice() };
    std::normal_distribution<double> mNorm{0, 1};
}

inline double sampleNormal()
{
    return mNorm(mGen);
}

class DiscreteDistribution
{
public:
    DiscreteDistribution(const std::vector<double>& weights)
    :
        mDistribution{weights.begin(), weights.end()}
    {
    }

    int sample()
    {
        return mDistribution(mGen);
    }

private:
    std::discrete_distribution<int> mDistribution;
};

}
