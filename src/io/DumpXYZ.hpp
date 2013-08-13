// ESPP_CLASS
#ifndef _IO_DUMPXYZ_HPP
#define _IO_DUMPXYZ_HPP

#include "ParticleAccess.hpp"
#include "integrator/MDIntegrator.hpp"

#include "esutil/Error.hpp"

#include <string>

namespace espresso {
  namespace io{

    class DumpXYZ : public ParticleAccess {

    public:

      DumpXYZ(shared_ptr<System> system, 
              shared_ptr<integrator::MDIntegrator> _integrator,
              std::string _file_name,
              bool _unfolded,
              real _length_factor,
              std::string _length_unit) :
                        ParticleAccess(system), 
                        integrator(_integrator),
                        file_name( _file_name ),
                        unfolded(_unfolded),
                        length_factor(_length_factor){ 
        setLengthUnit(_length_unit);
      }
      ~DumpXYZ() {}

      void perform_action(){
        dump();
      }
      
      void dump();
      
      std::string getFilename(){return file_name;}
      void setFilename(std::string v){file_name = v;}
      bool getUnfolded(){return unfolded;}
      void setUnfolded(bool v){unfolded = v;}

      std::string getLengthUnit(){return length_unit;}
      void setLengthUnit(std::string v){
        esutil::Error err( getSystem()->comm );
        if( v != "LJ" && v != "nm" && v != "A" ){
          std::stringstream msg;
          msg<<"Wrong unit length: "<< v << "  It should be string: LJ, nm or A" <<"\n";
          err.setException( msg.str() );
          err.checkException();
        }
        
        length_unit = v;
      }
      real getLengthFactor(){return length_factor;}
      void setLengthFactor(real v){length_factor = v;}
      
      static void registerPython();
    
    protected:

      //static LOG4ESPP_DECL_LOGGER(logger);

    private:
      
      // integrator we need to know an integration step
      shared_ptr<integrator::MDIntegrator> integrator;
      
      std::string file_name;
      
      bool unfolded;  // one can choose folded or unfolded coordinates, by default it is folded
      real length_factor;  // for example 
      std::string length_unit; // length unit: {could be LJ, nm, A} it is just for user info
    };
  }
}

#endif