#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <Eigen/Dense>

#include "filters/deterministicEKF.ipp"

namespace Filters::Python {

void bind_dynamic_dekf(pybind11::module& m) {
    pybind11::class_<DeterministicEKF::Dynamic>(m, "DeterministicEKF")
        .def(pybind11::init<
                 const typename DeterministicEKF::Dynamic::HiddenVector&,
                 const typename DeterministicEKF::Dynamic::HiddenMatrix&,
                 const typename DeterministicEKF::Dynamic::HiddenMatrix&,
                 const typename DeterministicEKF::Dynamic::HiddenMatrix&,
                 const typename DeterministicEKF::Dynamic::ObsMatrix&,
                 const typename DeterministicEKF::Dynamic::ObsNoiseMatrix&>(),
             pybind11::arg("stateEstimate"),
             pybind11::arg("covEstimate"),
             pybind11::arg("F"),
             pybind11::arg("Q"),
             pybind11::arg("H"),
             pybind11::arg("R"))
        .def("Update", &DeterministicEKF::Dynamic::Update, pybind11::arg("obs"))
        .def("Estimate", &DeterministicEKF::Dynamic::Estimate)
        .def("Predict", &DeterministicEKF::Dynamic::Predict)
        .def("GetLastInnovation", &DeterministicEKF::Dynamic::GetLastInnovation);
}

}  // namespace Filters::Python
