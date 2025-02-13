#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <Eigen/Dense>

#include "filters/stochasticEKF.ipp"

namespace Filters::Python {

void bind_dynamic_sekf(pybind11::module& m) {
    pybind11::class_<StochasticEKF::Dynamic>(m, "StochasticEKF")
        .def(pybind11::init<
                 const typename StochasticEKF::Dynamic::HiddenVector&,
                 const typename StochasticEKF::Dynamic::HiddenMatrix&,
                 const typename StochasticEKF::Dynamic::HiddenMatrix&,
                 const typename StochasticEKF::Dynamic::HiddenMatrix&,
                 const typename StochasticEKF::Dynamic::ObsMatrix&,
                 const typename StochasticEKF::Dynamic::ObsNoiseMatrix&>(),
             pybind11::arg("stateEstimate"),
             pybind11::arg("covEstimate"),
             pybind11::arg("F"),
             pybind11::arg("Q"),
             pybind11::arg("H"),
             pybind11::arg("R"))
        .def("Update", &StochasticEKF::Dynamic::Update, pybind11::arg("obs"))
        .def("Estimate", &StochasticEKF::Dynamic::Estimate)
        .def("Predict", &StochasticEKF::Dynamic::Predict)
        .def("GetLastInnovation", &StochasticEKF::Dynamic::GetLastInnovation);
}

}  // namespace Filters::Python
