#include "filters/particleFilter.hpp"

namespace Filters {

void ParticleFilter::Update(const Eigen::MatrixXd& obs)
{
    std::discrete_distribution<int> d(mWeights.begin(), mWeights.end());
    Eigen::MatrixXd newParticles(mNumParticles, mNumHidden);
    std::vector<double> probs(mNumParticles);
    for (unsigned i = 0; i < mNumParticles; ++i)
    {
        auto particle = mParticles.row(d(mGen));
        // Predict
        newParticles.block(i, 0, 1, mNumHidden) = mTransitionSampler(particle);
        // Estimate
        probs[i] = mProbabilityObserved(obs, particle);
    }

    mParticles = newParticles;
    double sumWeight = std::accumulate(probs.begin(), probs.end(), 0.);
    for (unsigned i = 0; i < mNumParticles; ++i)
        mWeights[i] = probs[i] / sumWeight;
}

}
