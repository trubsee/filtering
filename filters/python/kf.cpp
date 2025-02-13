#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <Eigen/Dense>

#include "filters/kalmanFilter.hpp"

namespace Filters::Python {

template <int Hidden, int Observed>
void bind_kalman(pybind11::module& m) {
    pybind11::class_<KalmanFilter::Static<Hidden, Observed>>(
        m,
        ("KalmanFilter_" + std::to_string(Hidden) + "_" +
         std::to_string(Observed))
            .c_str())
        .def(pybind11::init<
                 const typename KalmanFilter::Static<Hidden,
                                                     Observed>::HiddenVector&,
                 const typename KalmanFilter::Static<Hidden,
                                                     Observed>::HiddenMatrix&,
                 const typename KalmanFilter::Static<Hidden,
                                                     Observed>::HiddenMatrix&,
                 const typename KalmanFilter::Static<Hidden,
                                                     Observed>::HiddenMatrix&,
                 const typename KalmanFilter::Static<Hidden,
                                                     Observed>::ObsMatrix&,
                 const typename KalmanFilter::Static<Hidden, Observed>::
                     ObsNoiseMatrix&>(),
             pybind11::arg("stateEstimate"),
             pybind11::arg("covEstimate"),
             pybind11::arg("F"),
             pybind11::arg("Q"),
             pybind11::arg("H"),
             pybind11::arg("R"))
        .def("Update",
             &KalmanFilter::Static<Hidden, Observed>::Update,
             pybind11::arg("obs"))
        .def("Estimate", &KalmanFilter::Static<Hidden, Observed>::Estimate)
        .def("Predict", &KalmanFilter::Static<Hidden, Observed>::Predict)
        .def("GetLastInnovation",
             &KalmanFilter::Static<Hidden, Observed>::GetLastInnovation);
}

template <int hidden, int MaxHidden, int observed, int MaxObserved>
void bind_all_kalman(pybind11::module& m) {
    if constexpr (hidden <= MaxHidden && observed <= MaxObserved) {
        bind_kalman<hidden, observed>(m);
        bind_all_kalman<hidden + 1, MaxHidden, observed + 1, MaxObserved>(m);
    }
}

void bind_dynamic_kalman(pybind11::module& m) {
    pybind11::class_<KalmanFilter::Dynamic>(m, "KalmanFilter")
        .def(pybind11::init<
                 const typename KalmanFilter::Dynamic::HiddenVector&,
                 const typename KalmanFilter::Dynamic::HiddenMatrix&,
                 const typename KalmanFilter::Dynamic::HiddenMatrix&,
                 const typename KalmanFilter::Dynamic::HiddenMatrix&,
                 const typename KalmanFilter::Dynamic::ObsMatrix&,
                 const typename KalmanFilter::Dynamic::ObsNoiseMatrix&>(),
             pybind11::arg("stateEstimate"),
             pybind11::arg("covEstimate"),
             pybind11::arg("F"),
             pybind11::arg("Q"),
             pybind11::arg("H"),
             pybind11::arg("R"))
        .def("Update", &KalmanFilter::Dynamic::Update, pybind11::arg("obs"))
        .def("Estimate", &KalmanFilter::Dynamic::Estimate)
        .def("Predict", &KalmanFilter::Dynamic::Predict)
        .def("GetLastInnovation", &KalmanFilter::Dynamic::GetLastInnovation);
}

}  // namespace Filters::Python
