#ifndef ION7PROCESS_HH
#define ION7PROCESS_HH

#include "CH4IonizationProcess.hh"

//Ion_7 : CH₄ + e⁻ -> C⁺ + 2H₂ + 2e⁻
// 

class Ion7Process : public CH4IonizationProcess {

    public:

    Ion7Process() : CH4IonizationProcess("Ion7Process", 19.67*eV, {"C+", "H2", "H2"}){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon7(energy_eV/eV);
    }
        
};


#endif