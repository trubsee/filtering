#include <pybind11/pybind11.h>

#include "filters/python/kf.cpp"
#include "filters/python/sekf.cpp"

namespace Filters::Python {

PYBIND11_MODULE(fast_filters, m) {
    // bind_all_kalman<1, 3, 1, 3>(m);
    bind_dynamic_kalman(m);
    bind_dynamic_sekf(m);
}

}  // namespace Filters::Python
