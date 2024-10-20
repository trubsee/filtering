#pragma once
#include <functional>

#include <Eigen/Dense>

#include "common/assert.hpp"

namespace Filters {

class ParticleFilter
{
public:
    ParticleFilter(
        unsigned numberOfParticles,
        unsigned numberOfHiddenVariables,
        std::function<Eigen::MatrixXd()> const& initialSampler,
        std::function<Eigen::MatrixXd(const Eigen::MatrixXd&)> const& transitionSampler,
        std::function<double(const Eigen::MatrixXd&, const Eigen::MatrixXd&)> const& probabilityObserved
    )
    :
        mInitialSampler{initialSampler},
        mTransitionSampler{transitionSampler},
        mProbabilityObserved{probabilityObserved},
        mNumHidden{numberOfHiddenVariables},
        mNumParticles{numberOfParticles},
        mParticles(mNumParticles, mNumHidden),
        mWeights(mNumParticles),
        mEstimate{Eigen::MatrixXd::Zero(1, mNumHidden)}
    {
        initialiseParticles();
    }

    void Update(const Eigen::MatrixXd& obs);

    const Eigen::MatrixXd& Estimate() const { return mEstimate; }
private:
    void initialiseParticles()
    {
        for (unsigned i = 0; i < mNumParticles; ++i)
        {
            auto sample = mInitialSampler();
            ASSERT(sample.cols() == 1 && sample.rows() == mNumHidden);
            mParticles.block(i, 0, 1, mNumHidden) = sample.transpose();
            mWeights[i] = 1. / mNumParticles;
        }
    }
    
    std::function<Eigen::MatrixXd()> const mInitialSampler;
    std::function<Eigen::MatrixXd(const Eigen::MatrixXd&)> const mTransitionSampler;
    std::function<double(const Eigen::MatrixXd&, const Eigen::MatrixXd&)> const mProbabilityObserved;

    const unsigned mNumHidden;
    const unsigned mNumParticles;
    Eigen::MatrixXd mParticles;
    std::vector<double> mWeights;

    Eigen::MatrixXd mEstimate;
};

}
