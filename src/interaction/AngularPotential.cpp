#include "python.hpp"
#include "AngularPotential.hpp"

namespace espresso {
  namespace interaction {
    //////////////////////////////////////////////////
    // REGISTRATION WITH PYTHON
    //////////////////////////////////////////////////
    void
    AngularPotential::registerPython() {
      using namespace espresso::python;

      real (AngularPotential::*computeEnergy1)
        (const Real3D& dist12, const Real3D& dist32) const =
	&AngularPotential::computeEnergy;
      real (AngularPotential::*computeEnergy2)(real theta) const =
	&AngularPotential::computeEnergy;
      void (AngularPotential::*computeForce)
        (Real3D& force12, Real3D& force32,
        const Real3D& dist12, const Real3D& dist32) const =
	&AngularPotential::computeForce;

      class_< AngularPotential, boost::noncopyable >
	("interaction_AngularPotential", no_init)
	.add_property("cutoff",
		      &AngularPotential::getCutoff,
		      &AngularPotential::setCutoff)
	.def("computeEnergy", pure_virtual(computeEnergy1))
	.def("computeEnergy", pure_virtual(computeEnergy2))
	.def("computeForce", pure_virtual(computeForce))
	;
    }
  }
}