#include "stochasticModels/stochasticVolatility.hpp"

#include <cmath>

#include "common/constants.hpp"
#include "common/randomSample.hpp"

namespace StochasticModels {

StochasticVolatility::Vector StochasticVolatility::Mutate(
    const StochasticVolatility::Vector& input) const {
  return StochasticVolatility::Vector{
      {mCoef * std::exp(input(0) / 2) * Common::SampleNormal()}};
}

double StochasticVolatility::Probability(
    const StochasticVolatility::Vector& obs,
    const StochasticVolatility::Vector& hidden) const {
  const auto vol = mCoef * std::exp(hidden(0) / 2);
  return 1 / (2 * PI * vol) * std::exp(-std::pow(obs(0) / vol, 2) / 2);
}

}  // namespace StochasticModels
