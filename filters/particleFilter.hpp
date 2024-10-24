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
        mNumHidden{numberOfHiddenVariables},
        mNumParticles{numberOfParticles},
        // TODO: Profile if it is faster to store as a transpose
        mParticles(mNumParticles, mNumHidden),
        mWeights(mNumParticles),
        mEstimate{Eigen::MatrixXd::Zero(1, mNumHidden)},
        mInitialSampler{initialSampler},
        mTransitionSampler{transitionSampler},
        mProbabilityObserved{probabilityObserved}
    {
        InitialiseParticles();
    }

    void Update(const Eigen::MatrixXd& obs);

    const Eigen::MatrixXd& Estimate() const { return mEstimate; }

private:
    void InitialiseParticles();
    
    // estimate samples 
    const unsigned mNumHidden;
    const unsigned mNumParticles;
    Eigen::MatrixXd mParticles;
    std::vector<double> mWeights;
    Eigen::MatrixXd mEstimate;
    
    // model params
    std::function<Eigen::MatrixXd()> const mInitialSampler;
    std::function<Eigen::MatrixXd(const Eigen::MatrixXd&)> const mTransitionSampler;
    std::function<double(const Eigen::MatrixXd&, const Eigen::MatrixXd&)> const mProbabilityObserved;

};

}
