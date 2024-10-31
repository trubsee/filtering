#include <thread>
#include <mutex>

#include "common/statistics.hpp"
#include "filters/stochasticEKF.hpp"

namespace Filters {

template <int numSamples>
StochasticEKF<numSamples>::StochasticEKF(
    const Eigen::VectorXd& stateEstimate,
    const Eigen::MatrixXd& covEstimate,
    const StochasticModels::LinearGaussian& stateModel,
    const StochasticModels::LinearGaussian& obsModel
)
:
    mStateModel{stateModel},
    H{obsModel.GetCoefMatrix()},
    R{obsModel.GetNoiseMatrix()},
    mHidden{(unsigned)H.cols()},
    mObserved{(unsigned)H.rows()},
    mSamples{numSamples, Eigen::VectorXd{mHidden}},
    y{mObserved},
    S{mObserved, mObserved}
{
    ASSERT(stateEstimate.size() == mHidden);
    ASSERT(covEstimate.rows() == covEstimate.cols());
    ASSERT(covEstimate.rows() == mHidden);
    ASSERT(stateModel.GetNumOutputs() == stateModel.GetNumInputs());
    ASSERT(stateModel.GetNumOutputs() == mHidden);
    InitialiseSamples(stateEstimate, covEstimate);
}

template <int numSamples>
void StochasticEKF<numSamples>::Update(const Eigen::VectorXd& obs)
{
    ASSERT(obs.size() == mObserved);

    // Predict
    const auto xPred = Predict();
    Eigen::MatrixXd samples{numSamples, mHidden};
    for (unsigned i = 0; i < numSamples; ++i)
    {
        samples.block(i, 0, 1, mHidden) = mSamples[i].transpose();
    }
    const auto PPred = Common::CalculateCovariance(samples, samples);

    // Update
    y = obs - H * xPred;
    S = H * PPred * H.transpose() + R;
    const auto K = PPred * H.transpose() * S.inverse();
    mEstimate = xPred + K * y;
   
    UpdateSamples(obs, K);
    mPrediction = std::nullopt;
}

template <int numSamples>
const Eigen::VectorXd& StochasticEKF<numSamples>::Predict()
{
    if (mPrediction)
        return *mPrediction;

    Eigen::VectorXd sum{mHidden};
    for (unsigned i = 0; i < numSamples; ++i)
    {
        auto newSample = mStateModel.Mutate(mSamples[i]);
        mSamples[i] = newSample;
        sum += newSample;
    }
    mPrediction = sum / numSamples;
    return *mPrediction;
}

template <int numSamples>
void StochasticEKF<numSamples>::InitialiseSamples(
    const Eigen::VectorXd& stateEstimate,
    const Eigen::MatrixXd& covEstimate)
{
    const Eigen::MatrixXd tril = covEstimate.llt().matrixL();
    for (unsigned i = 0; i < numSamples; ++i)
    {
        mSamples[i] = stateEstimate + Common::SampleMvNormal(tril);
    }
}


template <int numSamples>
void StochasticEKF<numSamples>::UpdateSamples(const Eigen::VectorXd& obs, const Eigen::MatrixXd& K)
{
    const Eigen::MatrixXd tril = R.llt().matrixL();
    const auto updateBatch = [&tril, &K, &obs, this] (const Eigen::MatrixXd& x_i, int i)
    {
        Common::MvNormalSampler sampler;
        const auto v = sampler.SampleMvNormal(tril);
        mSamples[i] = x_i + K * (obs + v - H * x_i);
    };
   
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < numSamples; ++i)
    {
        const auto x_i = mSamples[i];
        threads.emplace_back(updateBatch, x_i, i);
    }

    for (auto& thread: threads)
    {
        thread.join();
    }
}

}
