#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <Eigen/Dense>

#include "filters/kalmanFilter.hpp"

namespace Filters {

template <int Hidden, int Observed>
void bind_kalman(pybind11::module& m) {
    // Binding the KalmanFilter class for the given Hidden and Observed sizes
    pybind11::class_<KalmanFilter<Hidden, Observed>>(
        m,
        ("KalmanFilter_" + std::to_string(Hidden) + "_" +
         std::to_string(Observed))
            .c_str())
        .def(pybind11::init<
                 const typename KalmanFilter<Hidden, Observed>::HiddenVector&,
                 const typename KalmanFilter<Hidden, Observed>::HiddenMatrix&,
                 const typename KalmanFilter<Hidden, Observed>::HiddenMatrix&,
                 const typename KalmanFilter<Hidden, Observed>::HiddenMatrix&,
                 const typename KalmanFilter<Hidden, Observed>::ObsMatrix&,
                 const typename KalmanFilter<Hidden,
                                             Observed>::ObsNoiseMatrix&>(),
             pybind11::arg("stateEstimate"),
             pybind11::arg("covEstimate"),
             pybind11::arg("F"),
             pybind11::arg("Q"),
             pybind11::arg("H"),
             pybind11::arg("R"))
        .def("Update",
             &KalmanFilter<Hidden, Observed>::Update,
             pybind11::arg("obs"))
        .def("Estimate", &KalmanFilter<Hidden, Observed>::Estimate)
        .def("Predict", &KalmanFilter<Hidden, Observed>::Predict)
        .def("GetLastInnovation",
             &KalmanFilter<Hidden, Observed>::GetLastInnovation);
}

template <int hidden, int MaxHidden, int observed, int MaxObserved>
void bind_all_kalman(pybind11::module& m) {
    if constexpr (hidden <= MaxHidden && observed <= MaxObserved) {
        bind_kalman<hidden, observed>(m);
        bind_all_kalman<hidden + 1, MaxHidden, observed + 1, MaxObserved>(m);
    }
}

PYBIND11_MODULE(pythonKF, m) { bind_all_kalman<1, 3, 1, 3>(m); }

}  // namespace Filters
