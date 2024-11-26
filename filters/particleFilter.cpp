#include "filters/particleFilter.hpp"

#include "common/randomSample.hpp"

namespace Filters {

void ParticleFilter::Update(const Eigen::MatrixXd& obs) {
    Common::DiscreteDistribution d{mWeights};
    Eigen::MatrixXd newParticles(mNumParticles, mNumHidden);
    std::vector<double> probs(mNumParticles);
    for (unsigned i = 0; i < mNumParticles; ++i) {
        auto particle = mParticles.row(d.Sample());
        // Predict
        newParticles.block(i, 0, 1, mNumHidden) = mTransitionSampler(particle);
        // Estimate
        probs[i] = mProbabilityObserved(obs, particle.transpose()) + 1e-300;
    }

    mParticles = newParticles;
    double sumWeight = std::accumulate(probs.begin(), probs.end(), 0.);
    mEstimate *= 0;
    for (unsigned i = 0; i < mNumParticles; ++i) {
        mWeights[i] = probs[i] / sumWeight;
        mEstimate += mWeights[i] * mParticles.block(i, 0, 1, mNumHidden);
    }
}

void ParticleFilter::InitialiseParticles() {
    for (unsigned i = 0; i < mNumParticles; ++i) {
        auto sample = mInitialSampler();
        ASSERT(sample.cols() == 1 && sample.rows() == mNumHidden);
        mParticles.block(i, 0, 1, mNumHidden) = sample.transpose();
        mWeights[i] = 1. / mNumParticles;
    }
}

}  // namespace Filters
