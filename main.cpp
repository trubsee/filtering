#include <Eigen/Dense>
#include <iostream>

int main() {
  Eigen::MatrixXd m1{{2, 2.5}, {-1, 0}};
  Eigen::MatrixXd m2{{1, 1}, {0, 1}};
  std::cout << m1 * m2 << std::endl;
}
