#ifndef ION5PROCESS_HH
#define ION5PROCESS_HH

#include "CH4IonizationProcess.hh"



//Ion_5 : CH₄ + e⁻ -> CH⁺ + H₂ + H* +  2e⁻
// 


class Ion5Process : public CH4IonizationProcess {
    public:

    Ion5Process()
    : CH4IonizationProcess("Ion5Process", 20.10*eV,{"CH+", "H2", "H*"}){}
    protected:
    G4double GetCrossSection(G4double energy_eV) const override {
        return GetCrossSectionTable().GetIon5(energy_eV/eV);
    }
        
};


#endif