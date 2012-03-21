"""
*****************************************
**Berendsen** - Berendsen barostat Object
*****************************************

This is the Berendsen barostat implementation according to the original paper [Berendsen84]_.
If Berendsen barostat is defined (as a property of integrator) then at the each run the system size
and the particle coordinates will be scaled by scaling parameter :math:`\mu` according to
the formula:

.. math::   \mu = [1 - \Delta t/\\tau (P_{0} - P)]^{1/3}

where :math:`\Delta t` - integration timestep, :math:`\\tau` - time parameter (coupling parameter),
:math:`P_{0}` - external pressure and :math:`P` - instantaneous pressure. 

Example:

    >>> berendsen = espresso.integrator.Berendsen(system)
    >>> berendsen.tau = 0.1
    >>> berendsen.pressure = 1.0
    >>> integrator.berendsen = berendsen

**!IMPORTANT** In order to run *npt* simulation one should separately define thermostat as well.

Definition:

    In order to define the Berendsen barostat
    
    >>> berendsen = espresso.integrator.Berendsen(system)
    
    one should have the System_ defined.

.. _System: espresso.System.html

Properties:

*   *berendsen.tau*

    The property 'tau' defines the time parameter :math:`\\tau`.

*   *berendsen.pressure*
    
    The property 'pressure' defines the external pressure :math:`P_{0}`.
    
Setting the integration property:
    
    >>> integrator.berendsen = berendsen
    
    It will define Berendsen barostat as a property of integrator.
    
One more example:

    >>> berendsen_barostat = espresso.integrator.Berendsen(system)
    >>> berendsen_barostat.tau = 10.0
    >>> berendsen_barostat.pressure = 3.5
    >>> integrator.berendsen = berendsen_barostat


Canceling the barostat:
    
    If one do not need the pressure regulation in system anymore or need to switch
    the ensamble or whatever :)

    >>> # define barostat with parameters
    >>> berendsen = espresso.integrator.Berendsen(system)
    >>> berendsen.tau = 0.8
    >>> berendsen.pressure = 15.0
    >>> integrator.berendsen = berendsen
    >>> ...
    >>> # some runs
    >>> ...
    >>> # erase Berendsen barostat
    >>> integrator.berendsen = None
    >>> # the next runs will not include the system size and particle coordinates scaling

References:

.. [Berendsen84] Berendsen et al., *J. Chem. Phys.*, 81, **1984**, p. 3684

"""


from espresso.esutil import cxxinit
from espresso import pmi

from _espresso import integrator_Berendsen

class BerendsenLocal(integrator_Berendsen):
  def __init__(self, system):
    'The (local) Velocity Verlet Integrator.'
    if not (pmi._PMIComm and pmi._PMIComm.isActive()) or pmi._MPIcomm.rank in pmi._PMIComm.getMPIcpugroup():
      cxxinit(self, integrator_Berendsen, system)

if pmi.isController:
  class Berendsen(object):
    __metaclass__ = pmi.Proxy
    pmiproxydefs = dict( cls =  'espresso.integrator.BerendsenLocal',
    pmiproperty = [ 'tau', 'pressure' ])