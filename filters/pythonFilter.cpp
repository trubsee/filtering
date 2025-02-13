#include <pybind11/pybind11.h>

#include "filters/python/kf.cpp"

namespace Filters::Python {

PYBIND11_MODULE(pythonKF, m) {
    // bind_all_kalman<1, 3, 1, 3>(m);
    bind_dynamic_kalman(m);
}

}  // namespace Filters::Python
