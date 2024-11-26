#include <cmath>

#include "filters/particleFilter.hpp"
#include "gtest/gtest.h"
#include "stochasticModels/linearFactoryMethods.hpp"

namespace Filters::Test {

TEST(ParticleFilterTest, CheckZeroDrift) {
  const Eigen::MatrixXd initial{{1}, {12}};
  const Eigen::MatrixXd covInit{{1, 0}, {0, 2}};
  auto rwInit{StochasticModels::CreateRandomWalk(covInit)};
  const Eigen::MatrixXd covNoise{{0.1, 0}, {0, 0.2}};
  auto rwNoise{StochasticModels::CreateRandomWalk(covNoise)};

  ParticleFilter pf{
      1000, 2, [&rwInit, &initial]() { return rwInit.Mutate(initial); },
      [](const Eigen::MatrixXd& input) { return input; },
      [&rwNoise](const Eigen::MatrixXd& obs, const Eigen::MatrixXd particle) {
        return rwNoise.Probability(obs, particle);
      }};

  for (unsigned i = 0; i < 100; ++i) {
    pf.Update(Eigen::MatrixXd{{1}, {12}});
  }
  // ParticleFilter Sufffers from Particle Impoverishment when there is no drift
  EXPECT_NEAR(pf.Estimate()(0, 0), 1, 0.1);
  EXPECT_NEAR(pf.Estimate()(0, 1), 12, 0.1);
}

}  // namespace Filters::Test
